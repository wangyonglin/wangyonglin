#if !defined(INCLUDE_WECHATHANDLER_H)
#define INCLUDE_WECHATHANDLER_H


#include <cJSON.h>


#include <aes_256_gcm.h>
#include <WechatHttps.h>
#include <base64.h>
#include <WebServer.h>


void WechatHandlerOpenId(struct evhttp_request *request, void *arg);

#endif