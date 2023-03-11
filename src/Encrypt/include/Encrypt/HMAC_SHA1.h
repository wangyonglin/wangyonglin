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

void hmac_sha1(const uint8_t *k,size_t lk,const uint8_t *d,size_t ld,uint8_t *out,size_t *t);
void HmacSha1_Base64(const uint8_t *k,size_t lk,const uint8_t *d,size_t ld,uint8_t **b64_encode_this,size_t encode_this_many_bytes);
#endif