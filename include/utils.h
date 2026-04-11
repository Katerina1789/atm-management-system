#ifndef UTILS_H
#define UTILS_H

// Remove trailing newline from fgets()
void trim_newline(char *s);

// Clear leftover input from stdin
void clear_input_buffer(void);

// Safe input reader
void read_line(char *buf, int size);

// Check if a string contains only digits
int is_number(const char *s);

// Simple screen spacer
void clear_screen(void);

#endif
