#if !defined(OPENSSL_HMAC_SHA1_H)
#define OPENSSL_HMAC_SHA1_H
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
#include <openssl/hmac.h>


#include <stdint.h>

void hmac_sha1(const uint8_t *k, /* secret key */
               size_t lk,        /* length of the key in bytes */
               const uint8_t *d, /* data */
               size_t ld,        /* length of data in bytes */
               uint8_t *out,     /* output buffer, at least "t" bytes */
               size_t *t);
#endif