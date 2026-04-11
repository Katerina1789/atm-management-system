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
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = load_accounts(accounts, MAX_ACCOUNTS);

    char buf[64];
    printf("Enter account number to update: ");
    read_line(buf, sizeof(buf));
    if (!is_number(buf)) {
        printf("Invalid account number.\n");
        return;
    }
    int target = atoi(buf);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_id == target &&
            accounts[i].user_id == ACTIVE_USER.id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    Account *a = &accounts[index];

    printf("New country (leave empty to keep): ");
    read_line(buf, sizeof(buf));
    if (strlen(buf) > 0) strcpy(a->country, buf);

    printf("New phone (leave empty to keep): ");
    read_line(buf, sizeof(buf));
    if (strlen(buf) > 0) strcpy(a->phone, buf);

    printf("New account type (leave empty to keep): ");
    read_line(buf, sizeof(buf));
    if (strlen(buf) > 0) strcpy(a->type, buf);

    if (!rewrite_accounts(accounts, count)) {
        printf("Error saving changes.\n");
        return;
    }

    printf("Account updated successfully.\n");
}

// Show account details with interest
void show_account_details(void) {
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = load_accounts(accounts, MAX_ACCOUNTS);

    char buf[64];
    printf("Enter account number: ");
    read_line(buf, sizeof(buf));
    if (!is_number(buf)) {
        printf("Invalid account number.\n");
        return;
    }
    int target = atoi(buf);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_id == target &&
            accounts[i].user_id == ACTIVE_USER.id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    Account *a = &accounts[index];

    double rate = 0.0;
    if (strcmp(a->type, "saving") == 0) rate = 0.05;
    if (strcmp(a->type, "current") == 0) rate = 0.02;
    if (strcmp(a->type, "fixed") == 0) rate = 0.07;

    double interest = a->balance * rate;

    printf("\nAccount ID: %d\n", a->account_id);
    printf("Type: %s\n", a->type);
    printf("Balance: %.2f\n", a->balance);
    printf("Interest: %.2f\n", interest);
    printf("Country: %s\n", a->country);
    printf("Phone: %s\n", a->phone);
    printf("Date: %s\n", a->date);
}

// Deposit / withdraw
void do_transaction(void) {
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = load_accounts(accounts, MAX_ACCOUNTS);

    char buf[64];
    printf("Enter account number: ");
    read_line(buf, sizeof(buf));
    if (!is_number(buf)) {
        printf("Invalid account number.\n");
        return;
    }
    int target = atoi(buf);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_id == target &&
            accounts[i].user_id == ACTIVE_USER.id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    Account *a = &accounts[index];

    printf("Enter amount (+deposit / -withdraw): ");
    read_line(buf, sizeof(buf));

    double amount = 0;
    if (sscanf(buf, "%lf", &amount) != 1) {
        printf("Invalid amount.\n");
        return;
    }

    if (amount < 0 && a->balance + amount < 0) {
        printf("Insufficient funds.\n");
        return;
    }

    a->balance += amount;

    if (!rewrite_accounts(accounts, count)) {
        printf("Error saving transaction.\n");
        return;
    }

    printf("Transaction successful. New balance: %.2f\n", a->balance);
}

// Delete an account
void delete_account(void) {
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int count = load_accounts(accounts, MAX_ACCOUNTS);

    char buf[64];
    printf("Enter account number to delete: ");
    read_line(buf, sizeof(buf));
    if (!is_number(buf)) {
        printf("Invalid account number.\n");
        return;
    }
    int target = atoi(buf);

    int index = -1;
    for (int i = 0; i < count; i++) {
        if (accounts[i].account_id == target &&
            accounts[i].user_id == ACTIVE_USER.id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Account not found.\n");
        return;
    }

    for (int i = index; i < count - 1; i++) {
        accounts[i] = accounts[i + 1];
    }
    count--;

    if (!rewrite_accounts(accounts, count)) {
        printf("Error deleting account.\n");
        return;
    }

    printf("Account deleted successfully.\n");
}

// Transfer ownership
void transfer_ownership(void) {
    printf("Transfer ownership (not implemented yet).\n");
}
