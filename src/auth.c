#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"
#include "notify.h"
#include "crypto.h"
#include "db.h"

User ACTIVE_USER;
int IS_LOGGED_IN = 0;

// Find user by username (SQLite)
int find_user_by_name(const char *name, User *out)
{
    return db_find_user_by_name(name, out);
}

// Register a new user
int register_user(void)
{
    char name[NAME_LEN];
    char pass[PASS_LEN];
    char hash[PASS_LEN];

    printf("Enter username: ");
    read_line(name, NAME_LEN);

    User tmp;
    if (find_user_by_name(name, &tmp))
    {
        printf("Username already exists.\n");
        return 0;
    }

    printf("Enter password: ");
    read_line(pass, PASS_LEN);

    hash_password(pass, hash, sizeof(hash));

    User users[MAX_USERS];
    int count = db_load_users(users, MAX_USERS);

    User u;
    u.id = (count > 0) ? users[count - 1].id + 1 : 1;

    strcpy(u.name, name);
    strcpy(u.password, hash);

    if (!db_insert_user(&u))
    {
        printf("Error saving user.\n");
        return 0;
    }

    printf("Registration successful.\n");
    return 1;
}

// Login user
int login_user(void)
{
    char name[NAME_LEN];
    char pass[PASS_LEN];
    char hash[PASS_LEN];

    printf("Username: ");
    read_line(name, NAME_LEN);

    User u;
    if (!find_user_by_name(name, &u))
    {
        printf("User not found.\n");
        return 0;
    }

    printf("Password: ");
    read_line(pass, PASS_LEN);

    hash_password(pass, hash, sizeof(hash));

    if (strcmp(hash, u.password) != 0)
    {
        printf("Incorrect password.\n");
        return 0;
    }

    ACTIVE_USER = u;
    IS_LOGGED_IN = 1;

    start_notification_listener(ACTIVE_USER.name);

    printf("Login successful.\n");
    return 1;
}
