# Architecture

Simple explanation of how the ATM Management System works.

## System Layers

```
┌─────────────────────┐
│   User Interface    │  Terminal with colors
└──────────┬──────────┘
           │
┌──────────▼──────────┐
│   Main Menu Loop    │  Handles user choices
└──────────┬──────────┘
           │
    ┌──────┴──────┐
    │             │
┌───▼───┐    ┌───▼────┐
│ Auth  │    │Accounts│  Business logic
└───┬───┘    └───┬────┘
    │            │
    └──────┬─────┘
           │
┌──────────▼──────────┐
│   SQLite Database   │  Data storage
└─────────────────────┘
```

## Main Flow

```
Start → Initialize DB → Loop:
                         ├─ Check notifications
                         ├─ Show menu
                         ├─ Get choice
                         ├─ Execute function
                         └─ Repeat
```

## Core Operations

### Register
```
Username → Check exists? → Hash password → Save → Done
```

### Login
```
Username → Find user → Verify password → Set session → Init notifications → Done
```

### Create Account
```
Logged in? → Input details → Validate → Save → Done
```

### Transaction
```
Find account → Own it? → Type OK? → Deposit/Withdraw → Update balance → Notify → Done
```

### Transfer
```
Find account → Own it? → Find new owner → Update owner → Notify recipient → Done
```

## Interest Calculation

```
savings:  balance × 7% ÷ 12        (monthly)
fixed01:  balance × 4%             (1 year)
fixed02:  balance × 5% × 2         (2 years)
fixed03:  balance × 8% × 3         (3 years)
current:  no interest
```

## Notifications

```
Event happens → Save to DB → User returns to menu → Display unread → Mark as read
```

**Types:** Account transfer, transactions, low balance, profile updates, password changes

## Module Structure

```
main.c      → Menu loop
auth.c      → Users (register, login, password)
accounts.c  → Accounts (create, update, delete, transfer, transactions)
db.c        → Database operations (SQLite)
notify.c    → Notifications
crypto.c    → Password hashing (FNV-1a)
tui.c       → Colors and display
utils.c     → Input helpers
```

## Database Tables

**users:** id, name, password, password_hash  
**accounts:** id, user_id, username, account_id, date, country, phone, balance, type  
**notifications:** id, username, message, timestamp, read

## Security

- Password hashing (FNV-1a)
- Ownership checks on all operations
- SQL injection prevention (prepared statements)
- Input validation

## Error Handling

Every operation checks:
1. User logged in?
2. Resource exists?
3. User owns it?
4. Input valid?
5. Operation succeeded?

If any fails → Show error → Return to menu

## Data Flow Example

```
1. Register → Save user
2. Login → Set session → Init notifications
3. Create account → Save account
4. Transfer account → Update owner → Notify recipient
5. Recipient logs in → See notification
```

## Summary

Simple pattern for all operations:
1. Validate (logged in, ownership, input)
2. Perform operation
3. Save to database
4. Send notification (if needed)
5. Show result
6. Return to menu
