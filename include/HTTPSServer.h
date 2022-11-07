#if !defined(INCLUDE_HTTPSSERVER_H)
#define INCLUDE_HTTPSSERVER_H
#include <string.h>
#include <event.h>
// for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <cJSON.h>
#include <unistd.h>
#include <SystemTypes.h>

typedef struct
{
    struct evhttp *httpd;
} httpd_config_t;

httpd_config_t *httpd_initializing(httpd_config_t **server, const char *address, uint16_t port);
ok_t httpd_router(httpd_config_t *server, void *this);
void httpd_cleanup(httpd_config_t *httpd);

void login_handler(struct evhttp_request *req, void *arg);
void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg);
#endif