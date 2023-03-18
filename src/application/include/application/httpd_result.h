#if !defined(INCLUDE_APPLICATION_HTTPD_RESULT_H)
#define INCLUDE_APPLICATION_HTTPD_RESULT_H
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
#include <application/aliapis.h>
#include <wangyonglin/buffer.h>
#include <application/httpd.h>
typedef struct _httpd_result_t
{
    struct evhttp_request *request;
    struct evkeyvalq params;
    httpd_t *httpd;
    localapis_t *aliapis;
    log_t *log;
    void *args;
} httpd_result_t;

httpd_result_t *httpd_result_create(httpd_result_t **result, log_t *log, struct evhttp_request *request, void *arg);
void httpd_result_delete(httpd_result_t *result);
void httpd_result_successful(httpd_result_t *result, string_t jsonstring);
void httpd_result_failure(httpd_result_t *result, char *errmsg);
char *httpd_result_get_params(httpd_result_t *result, const char *query);
string_t httpd_result_post_params(httpd_result_t *result);
#endif