#if !defined(INCLUDE_HTTPSUTILS_H)
#define INCLUDE_HTTPSUTILS_H

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <Stringex.h>

typedef struct _HTTPSUtilsCallback_t
{
    CURLcode code;
    char *memory;
    size_t size;
} HTTPSUtilsCallback;


Boolean HTTPSUtilsRequestGet(char *urlvalue,Stringex auth, HTTPSUtilsCallback *callback);
Boolean HTTPSUtilsRequestPost(char *urlvalue, Stringex auth, Stringex body, HTTPSUtilsCallback *callback);
#endif
