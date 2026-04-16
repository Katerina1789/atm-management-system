# ATM Management System — Product Requirements

## Overview

A command-line banking application in C demonstrating file I/O, data structures, memory management, and clean architecture.

## Data Storage

**users.txt** — `id name password`  
**records.txt** — `id user_id username account_id date country phone balance type`

## Mandatory Features

### 1. User Registration
- Unique usernames only
- Persist to `./data/users.txt`
- Reject duplicates with error message

### 2. User Login
- Validate credentials
- Load user session

### 3. Account Creation
- Fields: account_id, user_id, username, date, country, phone, balance, type
- Types: current, savings, fixed01, fixed02, fixed03
- Persist to `./data/records.txt`

### 4. List Accounts
- Show all accounts for logged-in user

### 5. Update Account
- Modify country or phone number
- Validate ownership
- Persist changes

### 6. Account Details
- Display account information
- Calculate interest:
  - **savings**: 7% monthly
  - **fixed01**: 4% yearly (1 year)
  - **fixed02**: 5% yearly (2 years)
  - **fixed03**: 8% yearly (3 years)
  - **current**: no interest

### 7. Transactions
- Deposit/withdraw for current and savings only
- Block transactions on fixed accounts
- Validate sufficient balance for withdrawals
- Persist updated balance

### 8. Delete Account
- Validate ownership
- Remove from storage

### 9. Transfer Ownership
- Transfer account to another user
- Validate target user exists
- Update user_id and username
- Persist changes

## Bonus Features

- **Real-time notifications** polling system
- **Password encryption** (simple hashing)
- **Improved TUI** with clear screens and consistent layout
- **Custom Makefile** with build, clean, debug targets
- **SQLite integration** replacing txt files

## Technical Requirements

- Written in C
- File-based persistence (or SQLite for bonus)
- Clean, modular code with minimal globals
- Safe memory handling
- Graceful error handling
- Must pass all audit criteria


## Project Approach

A structured, incremental workflow ensuring correctness, clarity, and audit-readiness.

### 1. Learn C Essentials

Focus only on language features required for this project:
- File I/O (fopen, fscanf, fprintf, fgets)
- Strings and memory (char[], pointers, malloc, free)
- Structs for users and accounts
- Basic control flow and error handling

### 2. Prepare Repository Structure

Create a clean, minimal layout:
- `src/` for modules
- `data/` for storage
- `Makefile` for build automation
- `docs/` for PRD, architecture, audit guide, structure

### 3. Implement Core Architecture

Follow a modular, predictable design:
- `auth.c` → user registration & login
- `system.c` → account operations
- `utils.c` → helpers (string, file, validation)
- `models.h` → structs for User and Account
- `io.c` → file read/write abstraction

Principles:
- No duplicated logic
- No global state except the active user
- Clear separation between data parsing, business logic, and UI
- Safe file rewriting (temp file → replace)

### 4. Implement Features Incrementally

Build features in the same order as the audit:
1. Registration
2. Login
3. Account creation
4. Listing
5. Updating
6. Details + interest
7. Transactions
8. Deletion
9. Ownership transfer

Each feature:
- Reads from file
- Validates input
- Updates file
- Returns clear success/error messages

### 5. Test Using Audit Scenarios

Use the audit guide as a checklist:
- Create users
- Create accounts
- Update fields
- Calculate interest
- Withdraw/deposit
- Delete accounts
- Transfer ownership

Testing method:
- After each feature, run the corresponding audit step
- Inspect `users.txt` and `records.txt` manually
- Fix logic before moving on

### 6. Refactor for Clarity

After all features work:
- Extract repeated code
- Improve naming
- Simplify control flow
- Add helper functions

Goal: minimal, readable, predictable C.

### 7. Add Bonus Features

Optional enhancements:
- Real-time notifications
- Password hashing
- Improved TUI
- Custom Makefile
- SQLite backend

### 8. Final Documentation

Produce clean, professional docs:
- `PRD.md`
- `architecture.md`
- `project_structure.md`
- `storage_implementation.md` (later along with more account types implementation)
- `audit_guide.md`

Documentation style:
- Minimal
- Structured
- Describes decisions, not emotions
- Matches the repo's clean aesthetic
