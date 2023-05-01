#if !defined(INCLUDE_HTTPDSERVER_H)
#define INCLUDE_HTTPDSERVER_H
#include <string.h>
#include <event.h>
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <unistd.h>
#include <cJSON.h>

#include <MainConfig.h>
#include <MemoryInject.h>
#include <AliyunHandler.h>

#include <WechatConfig.h>
#include <AliyunConfig.h>
#include <WechatConfigCertificates.h>
#include <zlog/zlog.h>

typedef struct _HTTPDServer_t
{
    struct evhttp *https;
    string_by_t address;
    integer_by_t port;
    integer_by_t timeout_in_secs;
    AliyunConfig *aliConfig;
    WechatConfig *wConfig;
    WechatConfigCertificates configCertificates;
    zlog_category_t *log;
} HTTPDServer;

HTTPDServer *HTTPDServerCreate(MainConfig  *mainConfig);
void HTTPDServerDelete();

void HTTPDServerStart(MainConfig  *mainConfig);
#endif