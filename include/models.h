#ifndef MODELS_H
#define MODELS_H

// Maximum number of records the program can load into memory
#define MAX_USERS 1000
#define MAX_ACCOUNTS 5000

// Maximum lengths for string fields (user input limits)
#define NAME_LEN 32
#define PASS_LEN 64
#define HASH_LEN 64
#define DATE_LEN 16
#define COUNTRY_LEN 32
#define PHONE_LEN 16
#define TYPE_LEN 16

// Represents a registered user
typedef struct
{
    int id;
    char name[NAME_LEN];
    char password[PASS_LEN];
    char password_hash[HASH_LEN];
} User;

// Represents a bank account belonging to a user
typedef struct
{
    int id;
    int user_id;
    char username[NAME_LEN];
    int account_id;
    char date[DATE_LEN];
    char country[COUNTRY_LEN];
    char phone[PHONE_LEN];
    double balance;
    char type[TYPE_LEN];
} Account;

// Global session state (defined in auth.c)
extern User ACTIVE_USER;
extern int IS_LOGGED_IN;

#endif
