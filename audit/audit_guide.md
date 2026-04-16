# Audit Guide

This document provides the official audit checklist for the ATM Management System.

## Pre-Audit Note

**Confirm the data storage method used in the project:**
- Text files (`./data/users.txt` and `./data/records.txt`)
- OR Relational database like SQLite (`./data/atm.db`)

**Current implementation:** SQLite Database

---

## Functional Tests

### 1. Register new user "Marcus"

**Test:**
1. Open the application
2. Register a new user with name `Marcus` and password `q1w2e3r4t5y6`

**Question:** Is this user saved in the data storage (text file `./data/users.txt` or database), and if so, are all credentials correct (name and password)?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT name, password FROM users WHERE name='Marcus';"`
- **Text file:** `cat data/users.txt | grep Marcus`

**Expected result:** ✅ User saved with correct name and password

---

### 2. Re-register existing user "Alice"

**Test:**
1. Open the application
2. Re-register the user `Alice`

**Question:** Did the application display an error message stating that this user already exists?

**Expected result:** ✅ Error message: "Username already exists."

---

### 3. Check unique usernames

**Test:**
1. Open the data storage (text file `./data/users.txt` or database)

**Question:** Are all the user names unique? (ex: no repetition on the name Alice)

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT name, COUNT(*) FROM users GROUP BY name HAVING COUNT(*) > 1;"`
- **Text file:** `cat data/users.txt | cut -d' ' -f2 | sort | uniq -d`

**Expected result:** ✅ No duplicate usernames (query returns nothing)

---

### 4. Login as "Alice"

**Test:**
1. Try and login as `Alice`

**Question:** Was Alice able to enter the main menu?

**Expected result:** ✅ Login successful, main menu displayed

---

### 5. Update non-existent account

**Test:**
1. Create two accounts with the user Alice
2. Select the option "Update Account"
3. Select an account number that does not exist for Alice

**Question:** Did the application display some kind of error message stating that this account does not exist?

**Expected result:** ✅ Error message: "Account not found."

---

### 6. Update account prompt

**Test:**
1. Resorting to the user Alice, try and select the option "Update Account"
2. Select one of the accounts you created for Alice

**Question:** Did the application prompt a choice of updating the phone number or the country?

**Expected result:** ✅ Prompt: "Which field do you want to update? (phone/country):"

---

### 7. Update phone number

**Test:**
1. Resorting to the user Alice, try and select the option "Update information of account"
2. Select one of the accounts you created for Alice
3. Then update the phone number of that account

**Question:** Was the phone number of that account updated in the application and the data storage (text file `records.txt` or database)?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT phone FROM accounts WHERE account_id=X;"`
- **Text file:** `cat data/records.txt | grep "account_id"`

**Expected result:** ✅ Phone number updated in storage

---

### 8. Update country

**Test:**
1. Resorting to the user Alice, try and select the option "Update Account"
2. Select one of the accounts you created for Alice
3. Then update the country of that account

**Question:** Was the country of that account updated in the application and the data storage (text file `records.txt` or database)?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT country FROM accounts WHERE account_id=X;"`
- **Text file:** `cat data/records.txt | grep "account_id"`

**Expected result:** ✅ Country updated in storage

---

### 9. Savings account interest

**Test:**
1. Resorting to the user Alice, try to create a new account with:
   - Account number: `834213`
   - Date: `10/10/2012`
   - Country: `UK`
   - Phone number: `291231392`
   - Deposit amount: `$1001.20`
   - Type of account: `savings`
2. Then select "Account Details" and choose the account you just created

**Question:** Did the application display the account information and the gain of $5.84 of interest on day 10 of every month?

**Expected result:** ✅ Display: "You will get $5.84 as interest on day 10 of every month."

---

### 10. Fixed01 account interest

**Test:**
1. Resorting to the user Alice create again an account but with:
   - Account number: `320421`
   - Type of account: `fixed01`
   - Rest of the information as in the last account
2. Then select "Account Details" and choose the account you just created

**Question:** Did the application display the account information and the gain of $40.05 of interest on 10/10/2013?

**Expected result:** ✅ Display: "You will get $40.05 as interest on 10/10/2013."

---

### 11. Fixed02 account interest

**Test:**
1. Resorting to the user Alice create again an account but with:
   - Account number: `3214`
   - Type of account: `fixed02`
   - Rest of the information as in the last account
2. Then select "Account Details" and choose the account you just created

**Question:** Did the application display the account information and the gain of $100.12 of interest on 10/10/2014?

**Expected result:** ✅ Display: "You will get $100.12 as interest on 10/10/2014."

---

### 12. Fixed03 account interest

**Test:**
1. Resorting to the user Alice create again an account but with:
   - Account number: `3212`
   - Type of account: `fixed03`
   - Rest of the information as in the last account
2. Then select "Account Details" and choose the account you just created

**Question:** Did the application display the account information and the gain of $240.29 of interest on 10/10/2015?

**Expected result:** ✅ Display: "You will get $240.29 as interest on 10/10/2015."

---

### 13. Fixed account transaction block

**Test:**
1. Resorting to the user Alice select the option "Transaction"
2. Then choose the account with the id `3212`

**Question:** Was an error message displayed stating it is not possible to withdraw or deposit for "fixed" accounts?

**Expected result:** ✅ Error message: "It is not possible to withdraw or deposit for fixed accounts."

---

### 14. Withdraw money

**Test:**
1. Resorting to the user Alice select the option "Transaction"
2. Choose the account with the id `834213`
3. Then try to withdraw money

**Questions:**
- Are you able to withdraw money?
- And if so, was the withdrawal updated in the data storage (text file `records.txt` or database)?
- Does the system forbid to withdraw an amount superior to your available balance?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT balance FROM accounts WHERE account_id=834213;"`
- **Text file:** `cat data/records.txt | grep 834213`

**Expected results:**
- ✅ Withdrawal successful
- ✅ Balance updated in storage
- ✅ Overdraft prevented with error message

---

### 15. Deposit money

**Test:**
1. Try to deposit money into the account `834213`

**Questions:**
- Were you able to deposit money into this account?
- And if so did it update the data storage (text file `records.txt` or database)?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT balance FROM accounts WHERE account_id=834213;"`
- **Text file:** `cat data/records.txt | grep 834213`

**Expected results:**
- ✅ Deposit successful
- ✅ Balance updated in storage

---

### 16. Remove accounts

**Test:**
1. Resorting to the user Alice try to select the option "Delete account"
2. Remove the accounts `834213`, `320421` and `3214`

**Question:** Can you confirm that those account were deleted, both in the application and data storage (text file `records.txt` or database)?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT * FROM accounts WHERE account_id IN (834213, 320421, 3214);"`
- **Text file:** `cat data/records.txt | grep -E "834213|320421|3214"`

**Expected result:** ✅ All three accounts deleted (query returns nothing)

---

### 17. Remove non-existent account

**Test:**
1. Resorting to the user Alice select the option "Delete account"
2. Try to remove an account that does not exist

**Question:** Did the application prompt some type of error saying that the account does not exist?

**Expected result:** ✅ Error message: "Account not found."

---

### 18. Transfer ownership

**Test:**
1. Create another user named `Michel`
2. Then by using Alice select the option "Transfer Ownership"
3. Try to transfer ownership of the account `3212` to Michel

**Question:** Were you able to transfer the ownership of this account to Michel? And if so did it update both application and data storage (text file `records.txt` or database)?

**How to verify:**
- **SQLite:** `sqlite3 data/atm.db "SELECT user_id, username FROM accounts WHERE account_id=3212;"`
- **Text file:** `cat data/records.txt | grep 3212`

**Expected result:** ✅ Ownership transferred, Michel is now the owner in storage

---

## Bonus

### Bonus 1: Real-time notifications

**Test:**
1. Open two terminals and login with two different users
2. Then transfer ownership of an account to the other user

**Question:** Was the user who received the account notified instantly?

**How to test:**
- Terminal 1: Login as Alice, transfer account to Michel
- Terminal 2: Login as Michel, return to menu or select "View Notifications"

**Expected result:** ✅ Michel sees notification: "You received account #X from Alice."

---

### Bonus 2: Terminal interface update

**Question:** Did the student update the terminal interface?

**How to test:**
- Check for colors in output
- Check for clear screen between operations
- Check for professional formatting

**Expected result:** ✅ Colored output, clear screens, improved UI

---

### Bonus 3: Password encryption

**Question:** Is the password saved in the data storage (text file `users.txt` or database) encrypted?

**How to test:**
- **SQLite:** `sqlite3 data/atm.db "SELECT password, password_hash FROM users LIMIT 1;"`
- **Text file:** `cat data/users.txt`

**Expected result:** ✅ Password hash field exists with encrypted value

---

### Bonus 4: Relational database

**Question:** Did the student create a relational database?

**How to test:**
- Check for `data/atm.db` file
- **SQLite:** `sqlite3 data/atm.db ".tables"`

**Expected result:** ✅ SQLite database with multiple tables (users, accounts, notifications)

---

### Bonus 5: Custom Makefile

**Question:** Did the student make their own Makefile?

**How to test:**
```bash
make clean
make all
./build/atm
```

**Expected result:** ✅ Makefile exists and compiles project successfully

---

### Bonus 6: Additional features

**Question:** Did the student add more features to the project?

**How to test:**
- Check for extra menu options
- Check for notification system
- Check for password change feature
- Check for notification history

**Expected result:** ✅ Features beyond basic requirements implemented

---

### Bonus 7: Code optimization

**Question:** Did the student optimize the code already given?

**How to test:**
- Check code organization (separate files for different functions)
- Check for modular design
- Check for proper error handling
- Check for memory management

**Expected result:** ✅ Clean, modular, well-organized code
