#if !defined(__USER_SALTBYTES_H__)
#define __USER_SALTBYTES_H__

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/base64.h"
#include <mbedtls/sha256.h>
typedef struct
{
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
    char *plaintext;
    size_t plainlen;
    unsigned char saltbytes[12];
    unsigned char salttext[32];
    size_t saltback;
} saltbytes_t;

int saltbytes_init(saltbytes_t **saltbytes, char *plaintext);
void saltbytes_free(saltbytes_t *saltbytes);
int saltbytes_sha256(saltbytes_t *saltbytes, unsigned char *ciphertext);
#endif