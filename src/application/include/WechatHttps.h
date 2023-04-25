#if !defined(INCLUDE_WECHATHTTPS_H)
#define INCLUDE_WECHATHTTPS_H
#include <string_by_this.h>
#include <WechatConfig.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <Stringex.h>
typedef struct _WechatHttpsCallback_t
{
    CURLcode code;
    char *memory;
    size_t size;
} WechatHttpsCallback;
long WechatHttpsPost(WechatHttpsCallback *callback, WechatConfig *wConfig, cJSON *postJSON, string_by_t authString, string_by_t url);
long WechatHttpsGet(WechatConfig *wConfig);

long WechatHttpsJscode2session(char * url, WechatHttpsCallback *callback);

long https_get_v3certificates(char *auth, WechatHttpsCallback *callback);

#endif