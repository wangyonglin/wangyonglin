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
#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/message.h>
#include <application/aliapis.h>
#include <wangyonglin/buffer.h>
typedef struct _httpd_t
{
    struct evhttp *https;
    char *address;
    long port;
    long timeout_in_secs;
    message_id_t msgid;
    localapis_t *aliapis;
    log_t *log;
} httpd_t;

void httpd_start(datasheet cfg,log_t *log);
void login_handler(struct evhttp_request *req, void *arg);
void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg);
void wireless_handler(struct evhttp_request *request, void *arg);
#endif