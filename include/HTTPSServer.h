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
#include <ConfUtils.h>
#include <SystemConfig.h>
typedef struct
{
    struct evhttp *httpd;
    char * address;
    long port;
    long timeout_in_secs;
    SystemConfig_t *SystemConfig;
} HTTPSServer_t;

ok_t HTTPSServer_initializing(HTTPSServer_t **HTTPSServer,SystemConfig_t *SystemConfig);
ok_t HTTPSServer_router(HTTPSServer_t *HTTPSServer);
void HTTPSServer_cleanup(HTTPSServer_t *HTTPSServer);

void login_handler(struct evhttp_request *req, void *arg);
void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg);
#endif