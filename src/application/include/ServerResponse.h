#if !defined(INCLUDE_SERVERRESPONSE_H)
#define INCLUDE_SERVERRESPONSE_H
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

#include <HTTPDServer.h>
#include <string_by_this.h>
#include <AliyunConfig.h>
#include <HTTPDResultCode.h>
#include <ResultUtil.h>
#include <Stringex.h>
#include <zlog/zlog.h>

typedef struct _ServerResponse_t
{
    struct evhttp_request *request;
    struct evkeyvalq params;
    HTTPDServer *httpd;
    AliyunConfig *aliConfig;
    void *args;
    Stringex body;
    zlog_category_t *log;
} ServerResponse;

ServerResponse *ServerResponseCreate(ServerResponse **response, zlog_category_t *log, struct evhttp_request *request, void *arg);
void ServerResponseDelete(ServerResponse *response);
Boolean ServerResponseBySuccessfly(ServerResponse *response, cJSON *responseBody);
Boolean ServerResponseByErrorMessage(ServerResponse *response, Integer responseCode, char *responseMessage);

void ServerResponseStringComplete(ServerResponse *response, int code, char *jsonstring);
void ServerResponseResultUtilComplete(ServerResponse *response, ResultUtil *ResUtil);
Boolean ServerResponseQuery(Stringex *retvalue, ServerResponse *response, const char *query_char);
Boolean ServerResponseHeadersValue(Stringex *retvalue, ServerResponse *response, const char *query_char);
#endif