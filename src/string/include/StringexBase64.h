#if !defined(INCLUDE_STRINGEXBASE64_H)
#define INCLUDE_STRINGEXBASE64_H
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
#include <Stringex.h>

#define BASE64_ENCODE_OUT_SIZE(datastring) ((unsigned int)((((datastring) + 2) / 3) * 4 + 1))
#define BASE64_DECODE_OUT_SIZE(datastring) ((unsigned int)(((datastring) / 4) * 3))

char *StringexBase64Encrypt(Stringex *callStringex,const void *b64_encode_this, int encode_this_many_bytes, Boolean with_new_line);
char *StringexBase64Decrypt(Stringex *callStringex, const void *b64_decode_this, int decode_this_many_bytes, Boolean with_new_line);
#endif