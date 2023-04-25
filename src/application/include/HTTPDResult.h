#if !defined(INCLUDE_HTTPDRESULT_H)
#define INCLUDE_HTTPDRESULT_H
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
#include <HTTPDServer.h>
#include <string_by_this.h>
#include <AliyunConfig.h>
#include <HTTPDResultCode.h>
#include <ResultUtil.h>


typedef struct _httpd_result_t
{
    struct evhttp_request *request;
    struct evkeyvalq params;
    HTTPDServer *httpd;
    AliyunConfig *aliConfig;
    void *args;
    cJSON * postJSON;
} httpd_result_t;

httpd_result_t *httpd_result_create(httpd_result_t **result, struct evhttp_request *request, void *arg);
void httpd_result_delete(httpd_result_t *result);
void httpd_result_complete(httpd_result_t *result, ResultUtil *ResUtil);
void httpd_complete(httpd_result_t *result, int code, char *jsonstring);
// void httpd_successful(httpd_result_t *result, string_by_t jsonstring);
// void httpd_failure(httpd_result_t *result, integer_by_t code, char *errmsg);
// void httpd_complete(httpd_result_t *result, integer_by_t code, string_by_t jsonstring);
// void httpd_complete_types(httpd_result_t *result, integer_by_t code, char *jsonstring, size_t jsonlength);

char *httpd_result_get_params(httpd_result_t *result, const char *query);
// string_t httpd_result_post_params(httpd_result_t *result);

#endif