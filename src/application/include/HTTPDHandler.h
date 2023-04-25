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
#define JSON_TYPE_RF433 "6B962DAC-3AB5-54A8-A197-A12E56FFEF8C"


void error_handler(struct evhttp_request *request, void *arg);
void HTTPDAliyunRegisterDevice(struct evhttp_request *request, void *arg);
void HTTPDAliyunGetDeviceStatus(struct evhttp_request *request, void *arg);

void HTTPDAliyunUserSet(struct evhttp_request *request, void *arg);

#endif