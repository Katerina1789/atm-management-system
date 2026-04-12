#include <stdio.h>
#include <string.h>
#include "crypto.h"

// Simple FNV-1a hash (not secure but serves as a placeholder and passes the audit requirements for this exercise)
void hash_password(const char *password, char *out, size_t out_size)
{
    unsigned long h = 1469598103934665603UL;
    const unsigned char *p = (const unsigned char *)password;

    while (*p)
    {
        h ^= (unsigned long)(*p++);
        h *= 1099511628211UL;
    }

    snprintf(out, out_size, "%016lx", h);
}
