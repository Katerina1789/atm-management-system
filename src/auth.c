#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "auth.h"
#include "utils.h"
#include "notify.h"
#include "crypto.h"
#include "db.h"

User ACTIVE_USER;
int IS_LOGGED_IN = 0;

// Find user by username
int find_user_by_name(const char *name, User *out)
{
    return db_find_user_by_name(name, out);
}

// Register a new user
int register_user(void)
{
    char name[NAME_LEN];
    char pass[PASS_LEN];
    char hash[HASH_LEN];

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
    strcpy(u.password, pass);
    strcpy(u.password_hash, hash);

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
    char hash[HASH_LEN];

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

    if (strcmp(pass, u.password) != 0 && strcmp(hash, u.password_hash) != 0)
    {
        printf("Incorrect password.\n");
        return 0;
    }

    ACTIVE_USER = u;
    IS_LOGGED_IN = 1;

    init_notifications(ACTIVE_USER.name);

    printf("Login successful.\n");
    return 1;
}

// Change password for logged-in user
void change_password(void)
{
    if (!IS_LOGGED_IN)
    {
        printf("You must be logged in.\n");
        return;
    }

    char old_pass[PASS_LEN];
    char new_pass[PASS_LEN];
    char confirm_pass[PASS_LEN];
    char old_hash[HASH_LEN];
    char new_hash[HASH_LEN];

    printf("Enter current password: ");
    read_line(old_pass, PASS_LEN);

    // Verify current password
    hash_password(old_pass, old_hash, sizeof(old_hash));
    if (strcmp(old_pass, ACTIVE_USER.password) != 0 && 
        strcmp(old_hash, ACTIVE_USER.password_hash) != 0)
    {
        printf("Incorrect current password.\n");
        return;
    }

    printf("Enter new password: ");
    read_line(new_pass, PASS_LEN);

    if (strlen(new_pass) < 4)
    {
        printf("Password must be at least 4 characters.\n");
        return;
    }

    printf("Confirm new password: ");
    read_line(confirm_pass, PASS_LEN);

    if (strcmp(new_pass, confirm_pass) != 0)
    {
        printf("Passwords do not match.\n");
        return;
    }

    // Hash new password
    hash_password(new_pass, new_hash, sizeof(new_hash));

    // Update in database
    sqlite3 *db;
    if (sqlite3_open("data/atm.db", &db) != SQLITE_OK)
    {
        printf("Error updating password.\n");
        return;
    }

    const char *sql = "UPDATE users SET password = ?, password_hash = ? WHERE id = ?;";
    sqlite3_stmt *stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        printf("Error updating password.\n");
        return;
    }

    sqlite3_bind_text(stmt, 1, new_pass, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, new_hash, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, ACTIVE_USER.id);

    if (sqlite3_step(stmt) == SQLITE_DONE)
    {
        // Update active user session
        strcpy(ACTIVE_USER.password, new_pass);
        strcpy(ACTIVE_USER.password_hash, new_hash);
        
        printf("Password changed successfully.\n");
        notify_password_change(ACTIVE_USER.name);
    }
    else
    {
        printf("Error updating password.\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
