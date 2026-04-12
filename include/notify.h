#ifndef NOTIFY_H
#define NOTIFY_H

// Initialize notification system for a user
void init_notifications(const char *username);

// Check for pending notifications and display them (auto-display unread)
void check_notifications(void);

// View all notifications (read and unread) - user-initiated
void view_all_notifications(void);

// Send notification about account transfer
void notify_account_transfer(const char *to_user, int account_id, const char *from_user);

#endif
