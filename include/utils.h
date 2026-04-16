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

// Validate date format (DD/MM/YYYY)
int is_valid_date(const char *date);

// Validate account type
int is_valid_account_type(const char *type);

// Validate country name
int is_valid_country(const char *country);

// Validate phone number (10 digits)
int is_valid_phone(const char *phone);

#endif
