#if !defined(INCLUDE_STRINGEXSHA256WITHRSA_H)
#define INCLUDE_STRINGEXSHA256WITHRSA_H
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <openssl/bio.h>
#include <string_by_this.h>
#include <Stringex.h>

Boolean Stringex_Sha256WithRSA_Signature(
    Stringex *base64String,
    Stringex apiclient_key,
    char *plainText);

Boolean Stringex_Sha256WithRSA_Verify(
    Stringex apiclient_key,
    Stringex plainText,
    Stringex ciphertext);

Boolean Stringex_Sha256WithRSA_Certificates_Verify(
    Stringex certificates,
    Stringex plainText,
    Stringex ciphertext);
#endif