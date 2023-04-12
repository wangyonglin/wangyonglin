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
#include <string_by_this.h>
void HTTPDrWechatJscode2session(struct evhttp_request *request, void *arg);
void HTTPDrWechatNotifyUrl(struct evhttp_request *request, void *arg);
void HTTPDrWechatPurchase(struct evhttp_request *request, void *arg);
void WehcatNotifyURLCreate(string_by_t *retstring ,boolean_by_t retbool);

void error_handler(struct evhttp_request *request, void *arg);
void HTTPDAliyunRegisterDevice(struct evhttp_request *request, void *arg);
void HTTPDAliyunGetDeviceStatus(struct evhttp_request *request, void *arg);
void HTTPDAliyunPub(struct evhttp_request *request, void *arg);
#endif