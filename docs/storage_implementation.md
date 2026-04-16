# Storage Implementation

## Current System: SQLite Database

**File:** `data/atm.db`

**Tables:**
- `users` - User accounts
- `accounts` - Bank accounts
- `notifications` - User notifications

**How it works:**
- All data stored in single database file
- Uses prepared statements (prevents SQL injection)
- ACID transactions ensure data integrity

## Alternative: Text Files

**Files:**
- `data/users.txt` - User data
- `data/records.txt` - Account data

**Format:**
```
users.txt:    id name password
records.txt:  id user_id username account_id date country phone balance type
```

**Status:** Code exists but not used (shows implementation without SQLite)

## Switching Storage Methods

Use the provided scripts to switch between SQLite and text files:

**Switch to text files:**
```bash
./scripts/migrate_to_textfiles.sh
```

**Switch back to SQLite:**
```bash
./scripts/migrate_to_sqlite.sh
```

**Note:** Switching to text files disables the notification system (requires SQLite).

## Summary

**Current:** SQLite (production-ready)  
**Alternative:** Text files (educational)  
**Scripts:** Automated migration both ways
