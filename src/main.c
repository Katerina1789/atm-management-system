#include <stdio.h>
#include <stdlib.h>
#include "auth.h"
#include "accounts.h"
#include "io.h"
#include "utils.h"

// Display the main menu
static void menu(void) {
    printf("\n--- ATM System ---\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("3. Create Account\n");
    printf("4. List Accounts\n");
    printf("5. Update Account\n");
    printf("6. Account Details\n");
    printf("7. Transaction\n");
    printf("8. Delete Account\n");
    printf("9. Transfer Ownership\n");
    printf("0. Exit\n");
    printf("Choice: ");
}

int main(void) {
    char buf[32];

    while (1) {
        menu();
        read_line(buf, sizeof(buf));

        switch (atoi(buf)) {
            case 1:
                register_user();
                break;
            case 2:
                login_user();
                break;
            case 3:
                create_account();
                break;
            case 4:
                list_accounts();
                break;
            case 5:
                update_account();
                break;
            case 6:
                show_account_details();
                break;
            case 7:
                do_transaction();
                break;
            case 8:
                delete_account();
                break;
            case 9:
                transfer_ownership();
                break;
            case 0:
                printf("Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
