#include <stdio.h>
#include <string.h>
#include "accounts.h"
#include "models.h"
#include "utils.h"
#include "io.h"
#include "auth.h"

// Create a new account
void create_account(void) {
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = load_accounts(accounts, MAX_ACCOUNTS);

    Account a;
    a.id = (count > 0) ? accounts[count - 1].id + 1 : 1;
    a.user_id = ACTIVE_USER.id;
    strcpy(a.username, ACTIVE_USER.name);

    char buf[64];

    printf("Enter account number: ");
    read_line(buf, sizeof(buf));
    if (!is_number(buf)) {
        printf("Invalid account number.\n");
        return;
    }
    a.account_id = atoi(buf);

    printf("Enter creation date (dd/mm/yyyy): ");
    read_line(a.date, DATE_LEN);

    printf("Enter country: ");
    read_line(a.country, COUNTRY_LEN);

    printf("Enter phone: ");
    read_line(a.phone, PHONE_LEN);

    printf("Enter initial balance: ");
    read_line(buf, sizeof(buf));
    if (sscanf(buf, "%lf", &a.balance) != 1) {
        printf("Invalid balance.\n");
        return;
    }

    printf("Enter account type: ");
    read_line(a.type, TYPE_LEN);

    if (!append_account(&a)) {
        printf("Error saving account.\n");
        return;
    }

    printf("Account created successfully.\n");
}

// List accounts for active user
void list_accounts(void) {
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = load_accounts(accounts, MAX_ACCOUNTS);

    int found = 0;

    for (int i = 0; i < count; i++) {
        if (accounts[i].user_id == ACTIVE_USER.id) {
            printf("\nAccount ID: %d\n", accounts[i].account_id);
            printf("Type: %s\n", accounts[i].type);
            printf("Balance: %.2f\n", accounts[i].balance);
            printf("Country: %s\n", accounts[i].country);
            printf("Phone: %s\n", accounts[i].phone);
            printf("Date: %s\n", accounts[i].date);
            found = 1;
        }
    }

    if (!found) {
        printf("No accounts found.\n");
    }
}

// Update an existing account
void update_account(void) {
    printf("Update account (not implemented yet).\n");
}

// Show account details with interest
void show_account_details(void) {
    printf("Account details (not implemented yet).\n");
}

// Deposit / withdraw
void do_transaction(void) {
    printf("Transaction (not implemented yet).\n");
}

// Delete an account
void delete_account(void) {
    printf("Delete account (not implemented yet).\n");
}

// Transfer ownership
void transfer_ownership(void) {
    printf("Transfer ownership (not implemented yet).\n");
}
