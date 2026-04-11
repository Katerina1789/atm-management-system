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

    printf("Which field do you want to update? (phone/country): ");
    read_line(buf, sizeof(buf));

    if (strcmp(buf, "phone") == 0) {
        printf("Enter new phone number: ");
        read_line(a->phone, PHONE_LEN);
    }
    else if (strcmp(buf, "country") == 0) {
        printf("Enter new country: ");
        read_line(a->country, COUNTRY_LEN);
    }
    else {
        printf("Invalid field.\n");
        return;
    }

    if (!rewrite_accounts(accounts, count)) {
        printf("Error saving changes.\n");
        return;
    }

    printf("Account updated successfully.\n");
}

// Show account details and interest information
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

    printf("\nAccount ID: %d\n", a->account_id);
    printf("Type: %s\n", a->type);
    printf("Balance: %.2f\n", a->balance);
    printf("Country: %s\n", a->country);
    printf("Phone: %s\n", a->phone);
    printf("Date: %s\n", a->date);

    int day = 0, month = 0, year = 0;
    sscanf(a->date, "%d/%d/%d", &day, &month, &year);

    double interest = 0.0;

    if (strcmp(a->type, "saving") == 0 || strcmp(a->type, "savings") == 0) {
        interest = a->balance * 0.07;
        printf("You will get $%.2f as interest on day %d of every month.\n",
               interest, day);
        return;
    }

    if (strcmp(a->type, "fixed01") == 0) {
        interest = a->balance * 0.04;
        printf("You will get $%.2f as interest on %02d/%02d/%04d.\n",
               interest, day, month, year + 1);
        return;
    }

    if (strcmp(a->type, "fixed02") == 0) {
        interest = a->balance * 0.05;
        printf("You will get $%.2f as interest on %02d/%02d/%04d.\n",
               interest, day, month, year + 2);
        return;
    }

    if (strcmp(a->type, "fixed03") == 0) {
        interest = a->balance * 0.08;
        printf("You will get $%.2f as interest on %02d/%02d/%04d.\n",
               interest, day, month, year + 3);
        return;
    }

    if (strcmp(a->type, "current") == 0) {
        printf("You will not get interests because the account is of type current.\n");
        return;
    }
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

    if (strcmp(a->type, "fixed01") == 0 ||
        strcmp(a->type, "fixed02") == 0 ||
        strcmp(a->type, "fixed03") == 0) {
        printf("It is not possible to withdraw or deposit for fixed accounts.\n");
        return;
    }

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
    if (!IS_LOGGED_IN) {
        printf("You must be logged in.\n");
        return;
    }

    Account accounts[MAX_ACCOUNTS];
    int acc_count = load_accounts(accounts, MAX_ACCOUNTS);

    User users[MAX_USERS];
    int user_count = load_users(users, MAX_USERS);

    char buf[64];

    printf("Enter account number to transfer: ");
    read_line(buf, sizeof(buf));
    if (!is_number(buf)) {
        printf("Invalid account number.\n");
        return;
    }
    int target_acc = atoi(buf);

    int acc_index = -1;
    for (int i = 0; i < acc_count; i++) {
        if (accounts[i].account_id == target_acc &&
            accounts[i].user_id == ACTIVE_USER.id) {
            acc_index = i;
            break;
        }
    }

    if (acc_index == -1) {
        printf("Account not found or not owned by you.\n");
        return;
    }

    printf("Enter username of new owner: ");
    read_line(buf, sizeof(buf));

    int new_owner_index = -1;
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, buf) == 0) {
            new_owner_index = i;
            break;
        }
    }

    if (new_owner_index == -1) {
        printf("Target user does not exist.\n");
        return;
    }

    accounts[acc_index].user_id = users[new_owner_index].id;
    strcpy(accounts[acc_index].username, users[new_owner_index].name);

    if (!rewrite_accounts(accounts, acc_count)) {
        printf("Error saving ownership transfer.\n");
        return;
    }

    printf("Ownership transferred successfully.\n");
}
