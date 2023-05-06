#if !defined(INCLUDE_HTTPD_H)
#define INCLUDE_HTTPD_H
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
#include <Tiktok/TiktokConfig.h>
typedef struct _WebServer
{
    struct evhttp *https;
    Stringex address;
    Integer port;
    Integer timeout_in_secs;
    AliyunConfig *aliConfig;
    WechatConfig *wConfig;
    WechatConfigCertificates configCertificates;
    TiktokConfig *tiktokConfig;
    zlog_category_t *log;
} WebServer,WebServer_t;

WebServer *WebServerCreate(MainConfig *mainConfig);
void WebServerDelete();

void WebServerStart(MainConfig *mainConfig);
#endif