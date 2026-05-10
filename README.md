# ATM Management System

[![C Language](https://img.shields.io/badge/C-11-A8B9CC?style=for-the-badge&logo=c&logoColor=white)](https://en.cppreference.com/w/c)
[![Zone01](https://img.shields.io/badge/Zone01-Athens-FF6B35?style=for-the-badge&logo=codeforces&logoColor=white)](https://zone01.gr/)
[![License](https://img.shields.io/badge/License-MIT-4caf50?style=for-the-badge&logo=opensourceinitiative&logoColor=white)](LICENSE)

A command-line banking application in C demonstrating file I/O, database operations, data structures, memory management, and clean architecture. Built as part of the Zone01 Athens curriculum.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Repository Structure](#repository-structure)
- [How to Run](#how-to-run)
- [Requirements](#requirements)
- [Usage](#usage)
- [Storage Systems](#storage-systems)
- [Documentation](#documentation)
- [License](#license)

## Description

ATM Management System is a terminal-based banking application that allows users to register, login, create accounts, perform transactions, and manage their banking operations. The system uses SQLite for data persistence and implements a comprehensive notification system for real-time updates.

## Features

### Mandatory Features

- **User Registration** - Create new users with unique usernames
- **User Login** - Secure authentication with password verification
- **Account Creation** - Create multiple account types (savings, current, fixed01-03)
- **List Accounts** - View all accounts owned by the logged-in user
- **Update Account** - Modify phone number and country information
- **Account Details** - View account information with interest calculations
- **Transactions** - Deposit and withdraw money with balance validation
- **Delete Account** - Remove accounts with ownership verification
- **Transfer Ownership** - Transfer account ownership to another user

### Bonus Features

- **SQLite Database** - Full database backend with 3 tables
- **Password Encryption** - FNV-1a hash algorithm with dual storage
- **Notification System** - Hybrid polling system with instant display and 30-day history
- **Improved TUI** - Colored terminal interface with professional formatting
- **Custom Makefile** - Professional build system with multiple targets
- **Change Password** - Secure password update with verification

### Notification Types

- Account ownership transfer
- Incoming transactions (deposits/withdrawals)
- Low balance warnings (< $100)
- Profile updates (phone/country changes)
- Password changes

## Repository Structure

```
atm-management-system/
├── audit/                   # Audit checklist 
├── data/                    # Data storage
├── docs/                    # Documentation
├── include/                 # Header files
├── src/                     # Source files
├── .gitignore
├── CONTRIBUTING.md
├── LICENSE
├── Makefile
└── README.md
```

## How to Run

### Quick Start

```bash
# Clone and navigate to project
cd atm-management-system

# Build the project
make

# Run the application
./build/atm
```

### Using Make Targets

```bash
# Build project
make all

# Build with debug symbols
make debug

# Build and run
make run

# Clean build artifacts
make clean
```

## Requirements

- **C Compiler**: GCC or Clang
- **SQLite3**: Version 3.x or higher
- **OS**: Linux, macOS, or Windows (with appropriate C compiler)
- **Make**: GNU Make (optional, for using Makefile)

### Installing SQLite3

**Linux (Debian/Ubuntu):**
```bash
sudo apt-get install libsqlite3-dev
```

**macOS:**
```bash
brew install sqlite3
```

**Linux (Fedora/RHEL):**
```bash
sudo dnf install sqlite-devel
```

## Usage

### Main Menu

```
1. Register          - Create new user account
2. Login             - Login to existing account
3. Create Account    - Create new bank account
4. List Accounts     - View all your accounts
5. Update Account    - Update phone or country
6. Account Details   - View account with interest
7. Transaction       - Deposit or withdraw money
8. Delete Account    - Remove an account
9. Transfer Ownership - Transfer account to another user
10. View Notifications - View notification history (when logged in)
11. Change Password   - Update your password (when logged in)
0. Exit              - Exit application
```

### Account Types

| Type | Interest Rate | Duration | Transactions |
|------|---------------|----------|--------------|
| **savings** | 7% annually (monthly) | Ongoing | Allowed |
| **current** | No interest | Ongoing | Allowed |
| **fixed01** | 4% annually | 1 year | Blocked |
| **fixed02** | 5% annually | 2 years | Blocked |
| **fixed03** | 8% annually | 3 years | Blocked |

### Interest Calculations

- **Savings**: Monthly interest = (balance × 0.07) ÷ 12
- **Fixed01**: Yearly interest = balance × 0.04
- **Fixed02**: 2-year interest = balance × 0.05 × 2
- **Fixed03**: 3-year interest = balance × 0.08 × 3
- **Current**: No interest

### Example Session

```bash
# Register a new user
Choice: 1
Enter username: Alice
Enter password: mypassword123
Registration successful.

# Login
Choice: 2
Username: Alice
Password: mypassword123
Login successful.

# Create a savings account
Choice: 3
Enter account number: 1001
Enter creation date (dd/mm/yyyy): 15/04/2024
Enter country: Greece
Enter phone: 1234567890
Enter initial balance: 5000.00
Enter account type: savings
Account created successfully.

# Check account details
Choice: 6
Enter account number: 1001

Account ID: 1001
Type: savings
Balance: 5000.00
Country: Greece
Phone: 1234567890
Date: 15/04/2024
You will get $29.17 as interest on day 15 of every month.
```

## Storage Systems

The application supports two storage methods:

### Current: SQLite Database (Default)

- **File**: `data/atm.db`
- **Features**: ACID transactions, notifications, fast queries
- **Recommended**: Production use

### Alternative: Text Files

- **Files**: `data/users.txt`, `data/records.txt`
- **Features**: Simple, human-readable
- **Use case**: Learning, debugging

### Switching Storage Methods (not currently supported)

**Switch to text files:**
```bash
./scripts/migrate_to_textfiles.sh
```

**Switch back to SQLite:**
```bash
./scripts/migrate_to_sqlite.sh
```

**Note**: Text file mode disables notifications (requires SQLite).

## Documentation

- [Architecture](docs/architecture.md) - System design and execution flows
- [Project Structure](docs/project_structure.md) - File descriptions and repository layout
- [Audit Guide](./audit/audit_guide.md) - Testing checklist and requirements
- [Contributing](CONTRIBUTING.md) - Contribution guidelines

## License

This project is licensed under the [MIT License](LICENSE).
