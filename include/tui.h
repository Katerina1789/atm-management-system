#ifndef TUI_H
#define TUI_H

#include <stddef.h>

// Colors
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN "\033[36m"

// Clear terminal screen
void tui_clear(void);

// Print a standard header
void tui_header(const char *title);

// Wait for user to press Enter
void tui_pause(void);

#endif
