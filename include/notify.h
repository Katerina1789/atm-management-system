#ifndef NOTIFY_H
#define NOTIFY_H

// Start background listener for incoming notifications
void start_notification_listener(const char *username);

// Send a notification message to a specific user
void send_notification(const char *username, const char *message);

#endif
