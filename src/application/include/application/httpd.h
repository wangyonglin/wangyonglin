#if !defined(INCLUDE_HTTPD_H)
#define INCLUDE_HTTPD_H
#include <string.h>
#include <event.h>
// for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <unistd.h>
#include <cJSON.h>
#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/message.h>

typedef struct _httpd_t
{
    struct evhttp *https;
    char *address;
    long port;
    long timeout_in_secs;
    app_t *app;
    message_id_t msgid;
} httpd_t;


ok_t httpd_create(app_t *app);
ok_t httpd_start();
void httpd_delete();

void login_handler(struct evhttp_request *req, void *arg);
void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg);
void wireless_handler(struct evhttp_request *request, void *arg);
#endif