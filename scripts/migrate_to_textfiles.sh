#!/bin/bash

# ATM Management System - Migrate to Text Files
# This script switches the project from SQLite to text file storage

set -e

echo "=========================================="
echo "  Migrating to Text File Storage"
echo "=========================================="
echo ""

# Check if already using text files
if [ -f "src/auth.c.sqlite_backup" ]; then
    echo "❌ Error: Already using text files!"
    echo "   Run migrate_to_sqlite.sh to switch back first."
    exit 1
fi

echo "📦 Creating backups..."
cp src/auth.c src/auth.c.sqlite_backup
cp src/accounts.c src/accounts.c.sqlite_backup
cp src/main.c src/main.c.sqlite_backup
cp Makefile Makefile.sqlite_backup
echo "   ✓ Backups created (.sqlite_backup)"
echo ""

echo "🔧 Updating auth.c..."
# Update includes
sed -i 's/#include "db\.h"/#include "io.h"/g' src/auth.c

# Update register_user function
sed -i 's/db_insert_user/append_user/g' src/auth.c

# Update find_user_by_name function
sed -i '/return db_find_user_by_name(name, out);/c\
    User users[MAX_USERS];\
    int count = load_users(users, MAX_USERS);\
    for (int i = 0; i < count; i++) {\
        if (strcmp(users[i].name, name) == 0) {\
            *out = users[i];\
            return 1;\
        }\
    }\
    return 0;' src/auth.c

# Update login_user function
sed -i 's/db_load_users/load_users/g' src/auth.c

echo "   ✓ auth.c updated"
echo ""

echo "🔧 Updating accounts.c..."
# Update includes
sed -i 's/#include "db\.h"/#include "io.h"/g' src/accounts.c

# Update function calls
sed -i 's/db_load_accounts/load_accounts/g' src/accounts.c
sed -i 's/db_insert_account/append_account/g' src/accounts.c

# Update db_update_account calls to rewrite_accounts
sed -i 's/if (!db_update_account(&accounts\[index\]))/if (!rewrite_accounts(accounts, count))/g' src/accounts.c
sed -i 's/if (!db_update_account(&accounts\[acc_index\]))/if (!rewrite_accounts(accounts, acc_count))/g' src/accounts.c
sed -i 's/if (!db_update_account(a))/if (!rewrite_accounts(accounts, count))/g' src/accounts.c

# Update db_delete_account to array removal + rewrite
sed -i '/if (!db_delete_account(accounts\[index\]\.id))/c\
    for (int i = index; i < count - 1; i++) {\
        accounts[i] = accounts[i + 1];\
    }\
    count--;\
    if (!rewrite_accounts(accounts, count))' src/accounts.c

echo "   ✓ accounts.c updated"
echo ""

echo "🔧 Updating main.c..."
# Comment out db_init
sed -i 's/db_init();/\/\/ db_init(); \/\/ Text file mode - no DB needed/g' src/main.c

# Comment out notification checks
sed -i 's/check_notifications();/\/\/ check_notifications(); \/\/ Disabled in text file mode/g' src/main.c

# Comment out notification includes
sed -i 's/#include "notify\.h"/\/\/ #include "notify.h" \/\/ Disabled in text file mode/g' src/main.c
sed -i 's/#include "db\.h"/\/\/ #include "db.h" \/\/ Using text files instead/g' src/main.c

# Comment out menu options 10 and 11
sed -i 's/printf(COLOR_YELLOW "10\. View Notifications/\/\/ printf(COLOR_YELLOW "10. View Notifications/g' src/main.c
sed -i 's/printf(COLOR_YELLOW "11\. Change Password/\/\/ printf(COLOR_YELLOW "11. Change Password/g' src/main.c

# Comment out cases 10 and 11
sed -i '/case 10:/,/break;/{s/^/\/\/ /}' src/main.c
sed -i '/case 11:/,/break;/{s/^/\/\/ /}' src/main.c

echo "   ✓ main.c updated"
echo ""

echo "🔧 Updating Makefile..."
# Remove SQLite linking
sed -i 's/-lsqlite3//g' Makefile
sed -i 's/-L\/home\/linuxbrew\/\.linuxbrew\/lib//g' Makefile

echo "   ✓ Makefile updated"
echo ""

echo "🏗️  Rebuilding project..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo "   ✓ Build successful"
echo ""

echo "✅ Migration complete!"
echo ""
echo "📝 Changes made:"
echo "   • Using text files: data/users.txt, data/records.txt"
echo "   • Notifications disabled (requires SQLite)"
echo "   • Menu options 10 & 11 disabled"
echo ""
echo "⚠️  Note: Create empty data files if they don't exist:"
echo "   touch data/users.txt data/records.txt"
echo ""
echo "🔄 To switch back to SQLite:"
echo "   ./scripts/migrate_to_sqlite.sh"
echo ""
