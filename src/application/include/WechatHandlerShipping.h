#if !defined(INCLUDE_WECHATHANDLSHIPPING_H)
#define INCLUDE_WECHATHANDLSHIPPING_H

#include <string_by_this.h>
#include <cJSON.h>
#include <HTTPDResult.h>
#include <string_by_this.h>
#include <string_by_hex.h>
#include <string_by_timestamp.h>
#include <string_by_hex.h>
#include <aes_256_gcm.h>
#include <WechatHttps.h>
#include <base64.h>
#include <HTTPDServer.h>
#include <SHA256WithRSA.h>

void WechatHandlerShipping(struct evhttp_request *request, void *arg);
#endif