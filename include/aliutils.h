#if !defined(INCLUDE_ALIUTILS_H)
#define INCLUDE_ALIUTILS_H
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wangyonglin/listitem.h>
#include <curl/curl.h>
char *alitimestamp(char **pointer);
char *aliutls_url_listitem(char **outputString, size_t outputStringMax, listitem *list);
int aliurls_base64(char **restypes, const int resmax, const char *strtypes, char *AccessKeySecret);
char * percentEncode(char ** dataOut,char * dataText,size_t dataSize);

void aliutils_https_get(char *url);
#endif