#include <stdio.h>
#include "tui.h"

// Clear screen
void tui_clear(void)
{
    printf("\033[2J\033[H");
}

// Print section header
void tui_header(const char *title)
{
    tui_clear();
    printf(COLOR_CYAN "=== %s ===\n\n" COLOR_RESET, title);
}

// Pause before returning to menu
void tui_pause(void)
{
    printf("\n" COLOR_YELLOW "Press Enter to continue..." COLOR_RESET);
    getchar();
}
