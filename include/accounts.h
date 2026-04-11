#ifndef ACCOUNTS_H
#define ACCOUNTS_H

// Create a new account
void create_account(void);

// List all accounts for the active user
void list_accounts(void);

// Update an existing account
void update_account(void);

// Show account details with interest
void show_account_details(void);

// Deposit / withdraw
void do_transaction(void);

// Delete an account
void delete_account(void);

// Transfer ownership to another user
void transfer_ownership(void);

#endif
