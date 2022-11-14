#if !defined(__COMPONENTS_STRING_INCLUDE_PUBLICUTILS_H__)
#define __COMPONENTS_STRING_INCLUDE_PUBLICUTILS_H__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>

#if !defined(PUBLICKUTILS_EXCEPTION)
#define PUBLICKUTILS_EXCEPTION
typedef int ok_t;
#define OK ((ok_t)1)
#define DoneException ((ok_t)1)
#define NoneException ((ok_t)0)
#define NullPointerException ((ok_t)-1)
#define ErrorException ((ok_t)-2)
#define ArgumentException ((ok_t)-3)
#define UnknownException ((ok_t)-4)
#endif

#endif