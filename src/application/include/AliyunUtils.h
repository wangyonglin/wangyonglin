#if !defined(INCLUDE_ALIYUNUTILS_H)
#define INCLUDE_ALIYUNUTILS_H

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ArrayList.h>
#include <curl/curl.h>



typedef struct _aliutils_apis_t
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
} aliutils_apis_t;


int aliurls_base64(char **restypes, const int resmax, const char *strtypes, char *AccessKeySecret);
char *percentEncode(char **dataOut, char *dataText, size_t dataSize);

void aliutils_https_get(char *url);

void aliutils_del(char *obj);

char *ReplaceFormat(char *formerString, size_t formerStringMax, const char findString, char *replaceString);
char *TopicFullNameFormat(char **outstring, char *ProductKey, char *DeviceName, char *TopicFullName);

// char * SignatureNonceFormat(char **outstring);
char *ContentBase64(char **outstring, char *MessageContentText, size_t MessageContentSize);
char *SignatureFormat(char **Signature, ArrayList lists[], size_t count, char *AccessKeySecret);
char *URLFormat(char **formerString, ArrayList lists[], size_t count, char *SignatureString);


#endif