#if !defined(INCLUDE_WANGYONGLIN_URL_H)
#define INCLUDE_WANGYONGLIN_URL_H
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <stdbool.h>

int UrlEncode(const char *str, char *result, const int max_length);
#endif