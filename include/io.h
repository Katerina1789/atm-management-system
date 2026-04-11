#ifndef IO_H
#define IO_H

#include "models.h"

// Load all users from file
int load_users(User *users, int max);

// Append a new user to file
int append_user(const User *u);

// Load all accounts from file
int load_accounts(Account *accounts, int max);

// Rewrite all accounts (safe replace)
int rewrite_accounts(Account *accounts, int count);

// Append a new account to file
int append_account(const Account *a);

#endif
