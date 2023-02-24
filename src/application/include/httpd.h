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
#include <wangyonglin/application.h>
#include <wangyonglin/message.h>

typedef struct
{
    struct evhttp *https;
    char *address;
    long port;
    long timeout_in_secs;
    app_t *app;
    message_id_t msgid;
} httpd_t;


ok_t httpd_initializing(httpd_t **httpd, app_t *app,char * conf);
ok_t httpd_start(httpd_t *httpd);
void httpd_destroy(httpd_t *httpd);

void login_handler(struct evhttp_request *req, void *arg);
void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg);
void wireless_handler(struct evhttp_request *request, void *arg);
#endif