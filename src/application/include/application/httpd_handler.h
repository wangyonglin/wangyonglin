#if !defined(INCLUDE_APPLICATION_HTTPD_HANDLER_H)
#define INCLUDE_APPLICATION_HTTPD_HANDLER_H
#include <string.h>
#include <event.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <unistd.h>
void PublishHandler(struct evhttp_request *request, void *arg);
void RegisterDeviceHandler(struct evhttp_request *request, void *arg);
void error_handler(struct evhttp_request *request, void *arg);
#endif