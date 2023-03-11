#if !defined(OPENSSL_BASE64_H)
#define OPENSSL_BASE64_H
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

char *base64encode(const void *b64_encode_this, int encode_this_many_bytes);
char *base64decode(const void *b64_decode_this, int decode_this_many_bytes);
#endif