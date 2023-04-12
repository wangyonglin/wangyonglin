#if !defined(INCLUDE_APPLICATION_ALIYUNHTTPSH)
#define INCLUDE_APPLICATION_ALIYUNHTTPSH

#include <wangyonglin/package.h>
#include <wangyonglin/wangyonglin.h>
#include <string_by_this.h>
#include <curl/curl.h>

typedef struct _AliyunHttpsCallback
{
    CURLcode code;
    char *memory;
    size_t size;
} AliyunHttpsCallback;

long AliyunHttpsGET(char *url, AliyunHttpsCallback *callback);
#endif