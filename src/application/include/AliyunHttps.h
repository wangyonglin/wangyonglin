#if !defined(INCLUDE_APPLICATION_ALIYUNHTTPSH)
#define INCLUDE_APPLICATION_ALIYUNHTTPSH

#include <Stringex.h>
#include <curl/curl.h>

typedef struct _AliyunHttpsCallback
{
    CURLcode code;
    char *memory;
    size_t size;
} AliyunHttpsCallback;

long AliyunHttpsGET(char *url, AliyunHttpsCallback *callback);
#endif