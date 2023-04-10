#if !defined(INCLUDE_HTTPS_RESULT_H)
#define INCLUDE_HTTPS_RESULT_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

typedef struct _https_result_t
{
    size_t jsonsize;
    size_t code;
    char *jsonformat;
} https_result;

void https_result_delete(https_result *result);
https_result *https_result_create(https_result **result);

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data);
#endif
