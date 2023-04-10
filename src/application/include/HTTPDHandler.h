#if !defined(INCLUDE_HTTPDHANDLER_H)
#define INCLUDE_HTTPDHANDLER_H
#include <string.h>
#include <event.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <unistd.h>

void HTTPDrWechatJscode2session(struct evhttp_request *request, void *arg);
void HTTPDrWechatNotifyUrl(struct evhttp_request *request, void *arg);
void HTTPDrWechatPurchase(struct evhttp_request *request, void *arg);

void error_handler(struct evhttp_request *request, void *arg);
void HTTPDAliyunRegisterDevice(struct evhttp_request *request, void *arg);
#endif