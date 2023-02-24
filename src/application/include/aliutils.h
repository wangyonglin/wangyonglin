#if !defined(INCLUDE_ALIUTILS_H)
#define INCLUDE_ALIUTILS_H
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <wangyonglin/list.h>
#include <curl/curl.h>
#include <wangyonglin/conf.h>
typedef struct _aliutils_sys
{
    char *DeviceName;
    char *AccessKeySecret;
    char *Action;
    char *ProductKey;
    char *TopicFullName;
    char *Format;
    char *Version;
    char *AccessKeyId;
    char *Signature;
    char *SignatureMethod;
    char *SignatureVersion;
    char *RegionId;
    long maxInflight;
} aliutils_sys;

typedef struct _aliutils_common
{
    char *Format;
    char *Version;
    char *AccessKeyId;
    char *Signature;
    char *SignatureMethod;
    char *Timestamp;
    char *SignatureVersion;
    char *SignatureNonce;
    char *RegionId;
} aliutils_common;

char *aliurls_timestamp(char **obj);
char *aliutls_url_list(char **outputString, size_t outputStringMax, list *objlist);
int aliurls_base64(char **restypes, const int resmax, const char *strtypes, char *AccessKeySecret);
char *percentEncode(char **dataOut, char *dataText, size_t dataSize);

void aliutils_https_get(char *url);

void aliutils_del(char *obj);
ok_t aliutils_sys_init(aliutils_sys **sys, const char *filename);
void aliutils_common_clean(aliutils_common *common);
aliutils_common * aliutils_common_init(aliutils_common **common, aliutils_sys *sys);
#endif