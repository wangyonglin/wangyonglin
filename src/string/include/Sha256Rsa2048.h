#if !defined(INCLUDE_SHA256_RSA2048_H)
#define INCLUDE_SHA256_RSA2048_H
#include <stdio.h>
#include <stdbool.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <Stringex.h>

Boolean Sha256Rsa2048_Signature(
    Stringex *ciphertext,
    Stringex certificates,
    char *plainText);

Boolean Sha256Rsa2048_Verify(
    Stringex ciphertext,
    Stringex plainText,
    Stringex certificates);
#endif