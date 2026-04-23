#ifndef AUTH_H
#define AUTH_H

#include "models.h"

// Find user by username
int find_user_by_name(const char *name, User *out);

// Register a new user
int register_user(void);

// Login an existing user
int login_user(void);

// Logout current user
void logout_user(void);

// Change password for logged-in user
void change_password(void);

#endif
