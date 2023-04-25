#if !defined(INCLUDE_OPENSSL_BASE64_H)
#define INCLUDE_OPENSSL_BASE64_H
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

char *base64_encrypt(const void *b64_encode_this, int encode_this_many_bytes, bool with_new_line);
char *base64_decrypt(const void *b64_decode_this, int decode_this_many_bytes, bool with_new_line);
#endif