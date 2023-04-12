#if !defined(INCLUDE_STDSSL_H)
#define INCLUDE_STDSSL_H
#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <stdio.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>
typedef unsigned char * bytes_t;
typedef struct _text_by_t{
 unsigned char * valuestring;
 size_t valuelength;
}text_by_t;

#endif