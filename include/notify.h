#ifndef NOTIFY_H
#define NOTIFY_H

// Initialize notification system for a user
void init_notifications(const char *username);

// Check for pending notifications and display them (auto-display unread)
void check_notifications(void);

// View all notifications (read and unread) - user-initiated
void view_all_notifications(void);

// Notification types
void notify_account_transfer(const char *to_user, int account_id, const char *from_user);
void notify_incoming_transaction(const char *to_user, int account_id, double amount, const char *type);
void notify_low_balance(const char *username, int account_id, double balance);
void notify_interest_applied(const char *username, int account_id, double interest);
void notify_profile_update(const char *username, int account_id, const char *field);
void notify_password_change(const char *username);

#endif
