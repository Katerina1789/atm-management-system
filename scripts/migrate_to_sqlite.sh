#!/bin/bash

# ATM Management System - Migrate to SQLite
# This script switches the project back to SQLite database storage

set -e

echo "=========================================="
echo "  Migrating to SQLite Database"
echo "=========================================="
echo ""

# Check if backups exist
if [ ! -f "src/auth.c.sqlite_backup" ]; then
    echo "❌ Error: No backups found!"
    echo "   This script restores from backups created by migrate_to_textfiles.sh"
    echo "   If you're already using SQLite, no action needed."
    exit 1
fi

echo "📦 Restoring from backups..."
cp src/auth.c.sqlite_backup src/auth.c
cp src/accounts.c.sqlite_backup src/accounts.c
cp src/main.c.sqlite_backup src/main.c
cp Makefile.sqlite_backup Makefile
echo "   ✓ Files restored"
echo ""

echo "🗑️  Removing backup files..."
rm src/auth.c.sqlite_backup
rm src/accounts.c.sqlite_backup
rm src/main.c.sqlite_backup
rm Makefile.sqlite_backup
echo "   ✓ Backups removed"
echo ""

echo "🏗️  Rebuilding project..."
make clean > /dev/null 2>&1
make > /dev/null 2>&1
echo "   ✓ Build successful"
echo ""

echo "✅ Migration complete!"
echo ""
echo "📝 Changes made:"
echo "   • Using SQLite database: data/atm.db"
echo "   • Notifications enabled"
echo "   • All menu options restored"
echo ""
echo "🔄 To switch back to text files:"
echo "   ./scripts/migrate_to_textfiles.sh"
echo ""
