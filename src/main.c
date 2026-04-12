#include <stdio.h>
#include <stdlib.h>
#include "auth.h"
#include "accounts.h"
#include "io.h"
#include "utils.h"
#include "tui.h"

// Display the main menu
static void menu(void)
{
    tui_header("ATM System");

    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Create Account\n");
    printf("4. List Accounts\n");
    printf("5. Update Account\n");
    printf("6. Account Details\n");
    printf("7. Transaction\n");
    printf("8. Delete Account\n");
    printf("9. Transfer Ownership\n");
    printf("0. Exit\n\n");
    printf("Choice: ");
}

int main(void)
{
    char buf[32];

    while (1)
    {
        menu();
        read_line(buf, sizeof(buf));

        switch (atoi(buf))
        {
        case 1:
            register_user();
            tui_pause();
            break;
        case 2:
            login_user();
            tui_pause();
            break;
        case 3:
            create_account();
            tui_pause();
            break;
        case 4:
            list_accounts();
            tui_pause();
            break;
        case 5:
            update_account();
            tui_pause();
            break;
        case 6:
            show_account_details();
            tui_pause();
            break;
        case 7:
            do_transaction();
            tui_pause();
            break;
        case 8:
            delete_account();
            tui_pause();
            break;
        case 9:
            transfer_ownership();
            tui_pause();
            break;
        case 0:
            printf(COLOR_GREEN "Goodbye!\n" COLOR_RESET);
            return 0;
        default:
            printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
            tui_pause();
        }
    }

    return 0;
}
