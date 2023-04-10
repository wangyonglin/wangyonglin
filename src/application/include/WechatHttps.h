#if !defined(INCLUDE_WECHATHTTPS_H)
#define INCLUDE_WECHATHTTPS_H
#include <string_by_this.h>
#include <WechatPayment.h>
#include <curl/curl.h>
typedef struct _WechatHttpsCallback_t
{
    CURLcode code;
    char *memory;
    size_t size;
} WechatHttpsCallback;
long WechatHttpsPost(WechatPayment *payment,string_by_t body, string_by_t authorization, string_by_t url, WechatHttpsCallback *callback);
long WechatHttpsGet(WechatPayment *payment);

long WechatHttpsJscode2session(char * url, WechatHttpsCallback *callback);
#endif