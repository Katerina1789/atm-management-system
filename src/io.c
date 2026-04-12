#include <stdio.h>
#include <string.h>
#include "io.h"

// Load users from users.txt
int load_users(User *users, int max)
{
    FILE *f = fopen("data/users.txt", "r");
    if (!f)
        return 0;

    int count = 0;
    while (count < max &&
           fscanf(f, "%d %31s %63s",
                  &users[count].id,
                  users[count].name,
                  users[count].password) == 3)
    {
        count++;
    }

    fclose(f);
    return count;
}

// Append a user to users.txt
int append_user(const User *u)
{
    FILE *f = fopen("data/users.txt", "a");
    if (!f)
        return 0;

    fprintf(f, "%d %s %s\n", u->id, u->name, u->password);
    fclose(f);
    return 1;
}

// Load accounts from records.txt
int load_accounts(Account *accounts, int max)
{
    FILE *f = fopen("data/records.txt", "r");
    if (!f)
        return 0;

    int count = 0;
    while (count < max &&
           fscanf(f, "%d %d %31s %d %15s %31s %15s %lf %15s",
                  &accounts[count].id,
                  &accounts[count].user_id,
                  accounts[count].username,
                  &accounts[count].account_id,
                  accounts[count].date,
                  accounts[count].country,
                  accounts[count].phone,
                  &accounts[count].balance,
                  accounts[count].type) == 9)
    {
        count++;
    }

    fclose(f);
    return count;
}

// Rewrite accounts safely using a temp file
int rewrite_accounts(Account *accounts, int count)
{
    FILE *f = fopen("data/records.tmp", "w");
    if (!f)
        return 0;

    for (int i = 0; i < count; i++)
    {
        fprintf(f, "%d %d %s %d %s %s %s %.2f %s\n",
                accounts[i].id,
                accounts[i].user_id,
                accounts[i].username,
                accounts[i].account_id,
                accounts[i].date,
                accounts[i].country,
                accounts[i].phone,
                accounts[i].balance,
                accounts[i].type);
    }

    fclose(f);

    remove("data/records.txt");
    rename("data/records.tmp", "data/records.txt");

    return 1;
}

// Append a new account to records.txt
int append_account(const Account *a)
{
    FILE *f = fopen("data/records.txt", "a");
    if (!f)
        return 0;

    fprintf(f, "%d %d %s %d %s %s %s %.2f %s\n",
            a->id,
            a->user_id,
            a->username,
            a->account_id,
            a->date,
            a->country,
            a->phone,
            a->balance,
            a->type);

    fclose(f);
    return 1;
}
