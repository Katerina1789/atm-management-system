#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "utils.h"

// Remove trailing newline from fgets()
void trim_newline(char *s)
{
    if (!s)
        return;
    size_t len = strlen(s);
    if (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r'))
    {
        s[len - 1] = '\0';
    }
}

// Flush leftover characters from stdin
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// Read a line safely and trim newline
void read_line(char *buf, int size)
{
    if (fgets(buf, size, stdin) != NULL)
    {
        trim_newline(buf);
    }
    else
    {
        buf[0] = '\0';
    }
}

// Check if a string is numeric
int is_number(const char *s)
{
    if (!s || *s == '\0')
        return 0;
    while (*s)
    {
        if (!isdigit((unsigned char)*s))
            return 0;
        s++;
    }
    return 1;
}

// Simple screen spacing
void clear_screen(void)
{
    printf("\n\n");
}

// Validate date format (DD/MM/YYYY)
int is_valid_date(const char *date)
{
    if (!date || strlen(date) != 10)
        return 0;

    if (date[2] != '/' || date[5] != '/')
        return 0;

    for (int i = 0; i < 10; i++)
    {
        if (i == 2 || i == 5)
            continue;
        if (!isdigit((unsigned char)date[i]))
            return 0;
    }

    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 +
               (date[8] - '0') * 10 + (date[9] - '0');

    if (day < 1 || day > 31)
        return 0;
    if (month < 1 || month > 12)
        return 0;
    if (year < 1900 || year > 2100)
        return 0;

    if (month == 2)
    {
        int is_leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > (is_leap ? 29 : 28))
            return 0;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        if (day > 30)
            return 0;
    }

    return 1;
}

// Validate account type
int is_valid_account_type(const char *type)
{
    if (!type)
        return 0;

    if (strcmp(type, "savings") == 0 ||
        strcmp(type, "current") == 0 ||
        strcmp(type, "fixed01") == 0 ||
        strcmp(type, "fixed02") == 0 ||
        strcmp(type, "fixed03") == 0)
    {
        return 1;
    }

    return 0;
}

// Validate country name
int is_valid_country(const char *country)
{
    if (!country || strlen(country) == 0)
        return 0;

    const char *valid_countries[] = {
        "Afghanistan", "Albania", "Algeria", "Andorra", "Angola",
        "AntiguaBarbuda", "Argentina", "Armenia", "Australia", "Austria",
        "Azerbaijan", "Bahamas", "Bahrain", "Bangladesh", "Barbados",
        "Belarus", "Belgium", "Belize", "Benin", "Bhutan",
        "Bolivia", "BosniaHerzegovina", "Botswana", "Brazil", "Brunei",
        "Bulgaria", "BurkinaFaso", "Burundi", "CaboVerde", "Cambodia",
        "Cameroon", "Canada", "CentralAfricanRepublic", "Chad", "Chile",
        "China", "Colombia", "Comoros", "Congo", "CostaRica",
        "Croatia", "Cuba", "Cyprus", "CzechRepublic", "Denmark",
        "Djibouti", "Dominica", "DominicanRepublic", "DRCongo", "Ecuador",
        "Egypt", "ElSalvador", "EquatorialGuinea", "Eritrea", "Estonia",
        "Eswatini", "Ethiopia", "Fiji", "Finland", "France",
        "Gabon", "Gambia", "Georgia", "Germany", "Ghana",
        "Greece", "Grenada", "Guatemala", "Guinea", "GuineaBissau",
        "Guyana", "Haiti", "Honduras", "Hungary", "Iceland",
        "India", "Indonesia", "Iran", "Iraq", "Ireland",
        "Israel", "Italy", "Jamaica", "Japan", "Jordan",
        "Kazakhstan", "Kenya", "Kiribati", "Kuwait", "Kyrgyzstan",
        "Laos", "Latvia", "Lebanon", "Lesotho", "Liberia",
        "Libya", "Liechtenstein", "Lithuania", "Luxembourg", "Madagascar",
        "Malawi", "Malaysia", "Maldives", "Mali", "Malta",
        "MarshallIslands", "Mauritania", "Mauritius", "Mexico", "Micronesia",
        "Moldova", "Monaco", "Mongolia", "Montenegro", "Morocco",
        "Mozambique", "Myanmar", "Namibia", "Nauru", "Nepal",
        "Netherlands", "NewZealand", "Nicaragua", "Niger", "Nigeria",
        "NorthKorea", "Norway", "Oman", "Pakistan",
        "Palau", "Palestine", "Panama", "PapuaNewGuinea", "Paraguay",
        "Peru", "Philippines", "Poland", "Portugal", "Qatar",
        "Romania", "Russia", "Rwanda", "SaintKittsNevis", "SaintLucia",
        "SaintVincentGrenadines", "Samoa", "SanMarino", "SaoTomePrincipe", "SaudiArabia",
        "Senegal", "Serbia", "Seychelles", "SierraLeone", "Singapore",
        "Slovakia", "Slovenia", "SolomonIslands", "Somalia", "SouthAfrica",
        "SouthKorea", "SouthSudan", "Spain", "SriLanka", "Sudan",
        "Suriname", "Sweden", "Switzerland", "Syria", "Taiwan",
        "Tajikistan", "Tanzania", "Thailand", "TimorLeste", "Togo",
        "Tonga", "TrinidadTobago", "Tunisia", "Turkey", "Turkmenistan",
        "Tuvalu", "Uganda", "Ukraine", "UnitedArabEmirates", "UnitedKingdom",
        "UnitedStates", "USA", "UK", "Uruguay", "Uzbekistan", "Vanuatu", "VaticanCity",
        "Venezuela", "Vietnam", "Yemen", "Zambia", "Zimbabwe"};

    int num_countries = sizeof(valid_countries) / sizeof(valid_countries[0]);

    for (int i = 0; i < num_countries; i++)
    {
        if (strcmp(country, valid_countries[i]) == 0)
            return 1;
    }

    return 0;
}

// Validate phone number (10 digits)
int is_valid_phone(const char *phone)
{
    if (!phone)
        return 0;

    int len = strlen(phone);
    if (len != 10)
        return 0;

    for (int i = 0; i < len; i++)
    {
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    }

    return 1;
}
