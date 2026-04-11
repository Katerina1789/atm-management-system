#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"
#include "io.h"

User ACTIVE_USER;
int IS_LOGGED_IN = 0;

// Find user by username
int find_user_by_name(const char *name, User *out) {
    User users[MAX_USERS];
    int count = load_users(users, MAX_USERS);

    for (int i = 0; i < count; i++) {
        if (strcmp(users[i].name, name) == 0) {
            if (out) *out = users[i];
            return 1;
        }
    }
    return 0;
}

// Register a new user
int register_user(void) {
    char name[NAME_LEN];
    char pass[PASS_LEN];

    printf("Enter username: ");
    read_line(name, NAME_LEN);

    User tmp;
    if (find_user_by_name(name, &tmp)) {
        printf("Username already exists.\n");
        return 0;
    }

    printf("Enter password: ");
    read_line(pass, PASS_LEN);

    User u;
    u.id = 1;

    User users[MAX_USERS];
    int count = load_users(users, MAX_USERS);
    if (count > 0) {
        u.id = users[count - 1].id + 1;
    }

    strcpy(u.name, name);
    strcpy(u.password, pass);

    if (!append_user(&u)) {
        printf("Error saving user.\n");
        return 0;
    }

    printf("Registration successful.\n");
    return 1;
}

// Login user
int login_user(void) {
    char name[NAME_LEN];
    char pass[PASS_LEN];

    printf("Username: ");
    read_line(name, NAME_LEN);

    User u;
    if (!find_user_by_name(name, &u)) {
        printf("User not found.\n");
        return 0;
    }

    printf("Password: ");
    read_line(pass, PASS_LEN);

    if (strcmp(pass, u.password) != 0) {
        printf("Incorrect password.\n");
        return 0;
    }

    ACTIVE_USER = u;
    IS_LOGGED_IN = 1;

    printf("Login successful.\n");
    return 1;
}
