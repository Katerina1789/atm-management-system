#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "notify.h"

// Build FIFO path for a given user
static void make_fifo_path(char *buf, size_t size, const char *username)
{
    snprintf(buf, size, "./data/notify_%s.fifo", username);
}

// Start background listener that prints notifications
void start_notification_listener(const char *username)
{
    char path[256];
    make_fifo_path(path, sizeof(path), username);

    mkfifo(path, 0666);

    pid_t pid = fork();
    if (pid != 0)
    {
        return;
    }

    // child: block and print messages
    while (1)
    {
        int fd = open(path, O_RDONLY);
        if (fd < 0)
        {
            exit(1);
        }

        char buf[256];
        ssize_t n = read(fd, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            printf("\n[Notification] %s\n", buf);
            fflush(stdout);
        }

        close(fd);
    }
}

// Send a notification message to another user's FIFO
void send_notification(const char *username, const char *message)
{
    char path[256];
    make_fifo_path(path, sizeof(path), username);

    int fd = open(path, O_WRONLY | O_NONBLOCK);
    if (fd < 0)
    {
        return; // user not listening
    }

    write(fd, message, strlen(message));
    close(fd);
}
