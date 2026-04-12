#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

// Remove trailing newline from fgets()
void trim_newline(char *s)
{
    if (!s)
        return;
    size_t len = strlen(s);
    if (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
    {
        s[len - 1] = '\0';
    }
}

// Flush leftover characters from stdin
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// Read a line safely and trim newline
void read_line(char *buf, int size)
{
    if (fgets(buf, size, stdin) != NULL)
    {
        trim_newline(buf);
    }
    else
    {
        buf[0] = '\0';
    }
}

// Check if a string is numeric
int is_number(const char *s)
{
    if (!s || *s == '\0')
        return 0;
    while (*s)
    {
        if (!isdigit((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}

// Simple screen spacing
void clear_screen(void)
{
    printf("\n\n");
}
