#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
#include "notify.h"
#include "db.h"
#include "models.h"
#include "tui.h"

static char CURRENT_USER[NAME_LEN] = "";
static time_t LAST_CHECK = 0;

// Initialize notification system for a user
void init_notifications(const char *username)
{
    strncpy(CURRENT_USER, username, NAME_LEN - 1);
    CURRENT_USER[NAME_LEN - 1] = '\0';
    LAST_CHECK = time(NULL);
}

// Check for pending notifications and display them
void check_notifications(void)
{
    if (CURRENT_USER[0] == '\0')
        return;

    sqlite3 *db;
    if (sqlite3_open("data/atm.db", &db) != SQLITE_OK)
        return;

    const char *sql =
        "SELECT id, message, timestamp FROM notifications "
        "WHERE username = ? AND read = 0 ORDER BY timestamp;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, CURRENT_USER, -1, SQLITE_STATIC);

    int has_notifications = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        if (!has_notifications)
        {
            printf("\n" COLOR_YELLOW "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" COLOR_RESET "\n");
            printf(COLOR_YELLOW "📬 NEW NOTIFICATIONS" COLOR_RESET "\n");
            printf(COLOR_YELLOW "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" COLOR_RESET "\n");
            has_notifications = 1;
        }

        int notif_id = sqlite3_column_int(stmt, 0);
        const char *message = (const char *)sqlite3_column_text(stmt, 1);
        printf(COLOR_GREEN "• %s" COLOR_RESET "\n", message);

        // Mark as read
        char update_sql[256];
        snprintf(update_sql, sizeof(update_sql),
                 "UPDATE notifications SET read = 1 WHERE id = %d;", notif_id);
        sqlite3_exec(db, update_sql, 0, 0, 0);
    }

    if (has_notifications)
    {
        printf(COLOR_YELLOW "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" COLOR_RESET "\n\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// View all notifications (read and unread) - user-initiated
void view_all_notifications(void)
{
    if (CURRENT_USER[0] == '\0')
    {
        printf("You must be logged in.\n");
        return;
    }

    sqlite3 *db;
    if (sqlite3_open("data/atm.db", &db) != SQLITE_OK)
    {
        printf("Error accessing notifications.\n");
        return;
    }

    // Get all notifications (read and unread) from last 30 days
    const char *sql =
        "SELECT id, message, timestamp, read FROM notifications "
        "WHERE username = ? AND timestamp > ? "
        "ORDER BY timestamp DESC;";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        printf("Error loading notifications.\n");
        return;
    }

    int thirty_days_ago = (int)time(NULL) - (30 * 24 * 60 * 60);

    sqlite3_bind_text(stmt, 1, CURRENT_USER, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, thirty_days_ago);

    int count = 0;
    printf("\n" COLOR_CYAN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" COLOR_RESET "\n");
    printf(COLOR_CYAN "📬 NOTIFICATION CENTER" COLOR_RESET "\n");
    printf(COLOR_CYAN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" COLOR_RESET "\n\n");

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int notif_id = sqlite3_column_int(stmt, 0);
        const char *message = (const char *)sqlite3_column_text(stmt, 1);
        int timestamp = sqlite3_column_int(stmt, 2);
        int is_read = sqlite3_column_int(stmt, 3);

        // Format timestamp
        time_t t = (time_t)timestamp;
        struct tm *tm_info = localtime(&t);
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M", tm_info);

        // Display with status indicator
        if (is_read)
        {
            printf(COLOR_RESET "  ✓ %s" COLOR_RESET " (%s)\n", message, time_str);
        }
        else
        {
            printf(COLOR_GREEN "  ● %s" COLOR_RESET " (%s) " COLOR_YELLOW "[NEW]" COLOR_RESET "\n", message, time_str);
            // Mark as read
            char update_sql[256];
            snprintf(update_sql, sizeof(update_sql),
                     "UPDATE notifications SET read = 1 WHERE id = %d;", notif_id);
            sqlite3_exec(db, update_sql, 0, 0, 0);
        }
        count++;
    }

    if (count == 0)
    {
        printf(COLOR_YELLOW "  No notifications in the last 30 days.\n" COLOR_RESET);
    }
    else
    {
        printf("\n" COLOR_CYAN "  Total: %d notification(s)" COLOR_RESET "\n", count);
    }

    printf(COLOR_CYAN "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" COLOR_RESET "\n");

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Send notification about account transfer
void notify_account_transfer(const char *to_user, int account_id, const char *from_user)
{
    sqlite3 *db;
    if (sqlite3_open("data/atm.db", &db) != SQLITE_OK)
        return;

    const char *sql =
        "INSERT INTO notifications (username, message, timestamp, read) "
        "VALUES (?, ?, ?, 0);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }

    char message[256];
    snprintf(message, sizeof(message),
             "You received account #%d from %s.", account_id, from_user);

    long long timestamp = (long long)time(NULL);

    sqlite3_bind_text(stmt, 1, to_user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, message, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, (int)timestamp);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Helper function to create notification
static void create_notification(const char *username, const char *message)
{
    sqlite3 *db;
    if (sqlite3_open("data/atm.db", &db) != SQLITE_OK)
        return;

    const char *sql =
        "INSERT INTO notifications (username, message, timestamp, read) "
        "VALUES (?, ?, ?, 0);";

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return;
    }

    long long timestamp = (long long)time(NULL);

    sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, message, -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, (int)timestamp);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

// Notify about incoming transaction (deposit from another user)
void notify_incoming_transaction(const char *to_user, int account_id, double amount, const char *type)
{
    char message[256];
    snprintf(message, sizeof(message),
             "Your account #%d received a %s of $%.2f.",
             account_id, type, amount);
    create_notification(to_user, message);
}

// Notify about low balance
void notify_low_balance(const char *username, int account_id, double balance)
{
    char message[256];
    snprintf(message, sizeof(message),
             "Warning: Account #%d balance is low ($%.2f).",
             account_id, balance);
    create_notification(username, message);
}

// Notify about interest applied
void notify_interest_applied(const char *username, int account_id, double interest)
{
    char message[256];
    snprintf(message, sizeof(message),
             "Interest of $%.2f has been applied to account #%d.",
             interest, account_id);
    create_notification(username, message);
}

// Notify about profile update
void notify_profile_update(const char *username, int account_id, const char *field)
{
    char message[256];
    snprintf(message, sizeof(message),
             "Account #%d %s has been updated.",
             account_id, field);
    create_notification(username, message);
}

// Notify about password change
void notify_password_change(const char *username)
{
    char message[256];
    snprintf(message, sizeof(message),
             "Your password has been changed successfully.");
    create_notification(username, message);
}
