#if !defined(INCLUDE_WECHATHANDLERNOTIFYURL_H)
#define INCLUDE_WECHATHANDLERNOTIFYURL_H


#include <cJSON.h>


#include <aes_256_gcm.h>
#include <WechatHttps.h>
#include <base64.h>
#include <WebServer.h>


void WechatHandlerNotifyUrl(struct evhttp_request *request, void *arg);
#endif