#if !defined(UTILS_INCLUDE_STRING_BY_BASE64_H)
#define UTILS_INCLUDE_STRING_BY_BASE64_H
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
#include <openssl/pem.h>
#include <string_by_this.h>
char *string_base64_encode(string_by_t *data, const void *b64_encode_this, int encode_this_many_bytes);
#endif