#ifndef DB_H
#define DB_H

#include "models.h"

// Initialize SQLite database and tables
int db_init(void);

// Users
int db_insert_user(const User *u);
int db_load_users(User *users, int max);
int db_find_user_by_name(const char *name, User *out);

// Accounts
int db_insert_account(const Account *a);
int db_load_accounts(Account *accounts, int max);
int db_update_account(const Account *a);
int db_delete_account(int id);

#endif
