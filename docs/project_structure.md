# Project Structure

Complete overview of all files and what they do.

## Directory Tree

```
atm-management-system/
├── data/
│   ├── atm.db              # SQLite database
│   ├── records.txt         # Backup: accounts (not used)
│   └── users.txt           # Backup: users (not used)
├── docs/
│   ├── architecture.md     # How the system works
│   ├── audit_guide.md      # Testing checklist
│   ├── prd.md              # Requirements
│   ├── project_structure.md # This file
│   └── storage_implementation.md # Database details
├── include/
│   ├── accounts.h          # Account functions
│   ├── auth.h              # Login/register functions
│   ├── crypto.h            # Password hashing
│   ├── db.h                # Database functions
│   ├── io.h                # File operations (not used)
│   ├── models.h            # Data structures
│   ├── notify.h            # Notifications
│   ├── tui.h               # Terminal colors
│   └── utils.h             # Helper functions
├── src/
│   ├── accounts.c          # Account operations
│   ├── auth.c              # Login/register
│   ├── crypto.c            # Password hashing
│   ├── db.c                # Database operations
│   ├── io.c                # File operations (not used)
│   ├── main.c              # Program entry point
│   ├── notify.c            # Notifications
│   ├── tui.c               # Terminal UI
│   └── utils.c             # Helper functions
├── .gitignore              # Git ignore rules
├── LICENSE                 # MIT License
├── Makefile                # Build instructions
└── README.md               # Project overview
```

---

## Core Files

### models.h

**What it is:** Defines data structures

**User Structure:**
```c
id          - User ID number
name        - Username
password    - Plain password
password_hash - Encrypted password
```

**Account Structure:**
```c
id          - Database ID
user_id     - Owner's ID
username    - Owner's name
account_id  - Account number
date        - Creation date
country     - Country
phone       - Phone number
balance     - Money amount
type        - Account type
```

**Global Variables:**
```c
ACTIVE_USER   - Currently logged in user
IS_LOGGED_IN  - True/false login status
```

---

## Module Pairs (Header + Source)

### auth.h + auth.c

**Purpose:** Handle users

**Functions:**
- `register_user()` - Create new user
- `login_user()` - Login existing user
- `change_password()` - Update password
- `find_user_by_name()` - Search for user

**How it works:**
1. Get username and password from user
2. Check database
3. Hash password if needed
4. Save or verify
5. Show result

---

### accounts.h + accounts.c

**Purpose:** Handle bank accounts

**Functions:**
- `create_account()` - Make new account
- `list_accounts()` - Show your accounts
- `update_account()` - Change phone/country
- `show_account_details()` - Show info + interest
- `do_transaction()` - Deposit/withdraw
- `delete_account()` - Remove account
- `transfer_ownership()` - Give account to someone

**How it works:**
1. Check if you're logged in
2. Get account number
3. Verify you own it
4. Do the operation
5. Save to database
6. Send notification if needed

---

### db.h + db.c

**Purpose:** Talk to SQLite database

**Functions:**
- `db_init()` - Create database and tables
- `db_insert_user()` - Add user
- `db_load_users()` - Get all users
- `db_find_user_by_name()` - Find user
- `db_insert_account()` - Add account
- `db_load_accounts()` - Get all accounts
- `db_update_account()` - Change account
- `db_delete_account()` - Remove account

**How it works:**
1. Open database connection
2. Prepare SQL query
3. Bind parameters (prevents SQL injection)
4. Execute query
5. Get results
6. Close connection

---

### notify.h + notify.c

**Purpose:** Send notifications

**Functions:**
- `init_notifications()` - Setup on login
- `check_notifications()` - Show unread messages
- `view_all_notifications()` - Show history
- `notify_account_transfer()` - Account received
- `notify_incoming_transaction()` - Money received
- `notify_low_balance()` - Balance warning
- `notify_profile_update()` - Info changed
- `notify_password_change()` - Password updated

**How it works:**
1. Event happens (transfer, transaction, etc.)
2. Create notification in database
3. When user returns to menu, check for new notifications
4. Display unread notifications
5. Mark as read

---

### crypto.h + crypto.c

**Purpose:** Encrypt passwords

**Functions:**
- `hash_password()` - Convert password to hash

**How it works:**
1. Take password string
2. Apply FNV-1a algorithm
3. Return 8-character hex string
4. Example: "password" → "3b1e4df4"

---

### tui.h + tui.c

**Purpose:** Make terminal look nice

**Functions:**
- `tui_clear()` - Clear screen
- `tui_header()` - Show title
- `tui_pause()` - Wait for Enter

**Colors:**
- Red - Errors
- Green - Success
- Yellow - Prompts
- Cyan - Headers

---

### utils.h + utils.c

**Purpose:** Helper functions

**Functions:**
- `read_line()` - Get user input safely
- `is_number()` - Check if string is number
- `trim_newline()` - Remove newline character

**How it works:**
- Prevents buffer overflow
- Validates input
- Cleans up strings

---

### io.h + io.c

**Purpose:** File operations (alternative to database)

**Status:** Not currently used, kept for future

**Functions:**
- `load_users()` - Read users.txt
- `append_user()` - Add to users.txt
- `load_accounts()` - Read records.txt
- `rewrite_accounts()` - Update records.txt
- `append_account()` - Add to records.txt

---

### main.c

**Purpose:** Program entry point

**What it does:**
1. Initialize database
2. Loop forever:
   - Check notifications
   - Show menu
   - Get user choice
   - Call appropriate function
   - Pause
3. Exit when user chooses 0

**Menu Options:**
```
1  - Register
2  - Login
3  - Create Account
4  - List Accounts
5  - Update Account
6  - Account Details
7  - Transaction
8  - Delete Account
9  - Transfer Ownership
10 - View Notifications (logged in only)
11 - Change Password (logged in only)
0  - Exit
```

---

## Data Directory

### data/atm.db

**What it is:** SQLite database file

**Contains:**
- users table
- accounts table
- notifications table

**Created by:** `db_init()` on first run

### data/users.txt

**What it is:** Text file backup (not used)

**Format:** `id name password`

**Example:**
```
0 Alice 1234password
1 Marcus q1w2e3r4t5y6
```

### data/records.txt

**What it is:** Text file backup (not used)

**Format:** `id user_id username account_id date country phone balance type`

**Example:**
```
0 0 Alice 100 10/10/2020 USA 1234567890 1001.20 savings
```

---

## Documentation Directory

### architecture.md
How the system is designed and how it works

### audit_guide.md
Step-by-step testing instructions

### prd.md
Product requirements and features

### project_structure.md
This file - explains all files

### storage_implementation.md
Database vs text files comparison

---

## Root Files

### .gitignore

**What it is:** Tells Git what to ignore

**Ignores:**
- build/ folder
- *.o files
- *.db files
- Temporary files

### LICENSE

**What it is:** MIT License

**Means:** Free to use, modify, distribute

### Makefile

**What it is:** Build automation

**Targets:**
- `make` or `make all` - Build project
- `make clean` - Remove build files
- `make debug` - Build with debug info
- `make run` - Build and run
- `make rebuild` - Clean and build

**How it works:**
1. Compile each .c file to .o file
2. Link all .o files together
3. Add SQLite library
4. Create executable in build/atm

### README.md

**What it is:** Project overview

**Contains:**
- Description
- Features
- How to run
- Usage examples
- Documentation links

---

## How Files Work Together

```
main.c
  ↓
Calls auth.c (register/login)
  ↓
Calls accounts.c (operations)
  ↓
Calls db.c (save/load)
  ↓
Calls notify.c (notifications)
  ↓
Uses tui.c (colors)
  ↓
Uses utils.c (input)
  ↓
Uses crypto.c (passwords)
```

---

## Summary

**9 source files** - Each does one thing
- main.c - Menu loop
- auth.c - Users
- accounts.c - Accounts
- db.c - Database
- notify.c - Notifications
- crypto.c - Passwords
- tui.c - Colors
- utils.c - Helpers
- io.c - Files (backup)

**9 header files** - Function declarations

**1 database** - atm.db (SQLite)

**5 docs** - Explain everything

**1 Makefile** - Build automation

**Total: 28 files**

Everything is organized, simple, and easy to understand.
