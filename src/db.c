#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sqlite3.h>
#include "db.h"

static const char *DB_PATH = "data/atm.db";

static int open_db(sqlite3 **db)
{
    if (sqlite3_open(DB_PATH, db) != SQLITE_OK)
        return 0;
    return 1;
}

static void ensure_data_dir(void)
{
    mkdir("data", 0755);
}

// Initialize database and tables
int db_init(void)
{
    ensure_data_dir();

    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *users_sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY,"
        "name TEXT UNIQUE,"
        "password TEXT,"
        "password_hash TEXT"
        ");";

    const char *accounts_sql =
        "CREATE TABLE IF NOT EXISTS accounts ("
        "id INTEGER PRIMARY KEY,"
        "user_id INTEGER,"
        "username TEXT,"
        "account_id INTEGER,"
        "date TEXT,"
        "country TEXT,"
        "phone TEXT,"
        "balance REAL,"
        "type TEXT"
        ");";

    const char *notifications_sql =
        "CREATE TABLE IF NOT EXISTS notifications ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "username TEXT,"
        "message TEXT,"
        "timestamp INTEGER,"
        "read INTEGER DEFAULT 0"
        ");";

    sqlite3_exec(db, users_sql, 0, 0, 0);
    sqlite3_exec(db, accounts_sql, 0, 0, 0);
    sqlite3_exec(db, notifications_sql, 0, 0, 0);

    sqlite3_close(db);
    return 1;
}

// Insert user
int db_insert_user(const User *u)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql =
        "INSERT INTO users (id, name, password, password_hash) VALUES (?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, u->id);
    sqlite3_bind_text(stmt, 2, u->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, u->password, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, u->password_hash, -1, SQLITE_STATIC);

    int ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Load users
int db_load_users(User *users, int max)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql = "SELECT id, name, password, password_hash FROM users ORDER BY id;";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max)
    {
        users[count].id = sqlite3_column_int(stmt, 0);
        strcpy(users[count].name, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(users[count].password, (const char *)sqlite3_column_text(stmt, 2));
        const char *hash = (const char *)sqlite3_column_text(stmt, 3);
        if (hash)
            strcpy(users[count].password_hash, hash);
        else
            users[count].password_hash[0] = '\0';
        count++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return count;
}

// Find user by name
int db_find_user_by_name(const char *name, User *out)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql =
        "SELECT id, name, password, password_hash FROM users WHERE name = ? LIMIT 1;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);

    int found = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        out->id = sqlite3_column_int(stmt, 0);
        strcpy(out->name, (const char *)sqlite3_column_text(stmt, 1));
        strcpy(out->password, (const char *)sqlite3_column_text(stmt, 2));
        const char *hash = (const char *)sqlite3_column_text(stmt, 3);
        if (hash)
            strcpy(out->password_hash, hash);
        else
            out->password_hash[0] = '\0';
        found = 1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return found;
}

// Insert account
int db_insert_account(const Account *a)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql =
        "INSERT INTO accounts "
        "(id, user_id, username, account_id, date, country, phone, balance, type) "
        "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, a->id);
    sqlite3_bind_int(stmt, 2, a->user_id);
    sqlite3_bind_text(stmt, 3, a->username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, a->account_id);
    sqlite3_bind_text(stmt, 5, a->date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, a->country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 7, a->phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 8, a->balance);
    sqlite3_bind_text(stmt, 9, a->type, -1, SQLITE_STATIC);

    int ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Load accounts
int db_load_accounts(Account *accounts, int max)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql =
        "SELECT id, user_id, username, account_id, date, country, phone, balance, type "
        "FROM accounts ORDER BY id;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && count < max)
    {
        accounts[count].id = sqlite3_column_int(stmt, 0);
        accounts[count].user_id = sqlite3_column_int(stmt, 1);
        strcpy(accounts[count].username, (const char *)sqlite3_column_text(stmt, 2));
        accounts[count].account_id = sqlite3_column_int(stmt, 3);
        strcpy(accounts[count].date, (const char *)sqlite3_column_text(stmt, 4));
        strcpy(accounts[count].country, (const char *)sqlite3_column_text(stmt, 5));
        strcpy(accounts[count].phone, (const char *)sqlite3_column_text(stmt, 6));
        accounts[count].balance = sqlite3_column_double(stmt, 7);
        strcpy(accounts[count].type, (const char *)sqlite3_column_text(stmt, 8));
        count++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return count;
}

// Update account
int db_update_account(const Account *a)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql =
        "UPDATE accounts SET "
        "user_id = ?, username = ?, account_id = ?, date = ?, "
        "country = ?, phone = ?, balance = ?, type = ? "
        "WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, a->user_id);
    sqlite3_bind_text(stmt, 2, a->username, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, a->account_id);
    sqlite3_bind_text(stmt, 4, a->date, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, a->country, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 6, a->phone, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 7, a->balance);
    sqlite3_bind_text(stmt, 8, a->type, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 9, a->id);

    int ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}

// Delete account by id
int db_delete_account(int id)
{
    sqlite3 *db;
    if (!open_db(&db))
        return 0;

    const char *sql = "DELETE FROM accounts WHERE id = ?;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return 0;
    }

    sqlite3_bind_int(stmt, 1, id);

    int ok = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return ok;
}
