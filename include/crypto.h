#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>

// Compute a simple hash of the password into a hex string
void hash_password(const char *password, char *out, size_t out_size);

#endif
