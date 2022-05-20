#include <event.h>
// for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include "json/cJSON.h"
#include <unistd.h>
#include <wangyonglin/string.h>
#include <https/httpd.h>
#include <json/cJSON_Results.h>


void login_handler(struct evhttp_request *req, void *arg)
{
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");

    cJSON_Results_t *obj;
    char *out;
    //获取POST方法的数据
    size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
    char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);

    if (post_size > 0 && post_size < 1024)
    {
        obj = cJSON_Results_new(400);
    }
    else
    {
        obj = cJSON_Results_new(200);
    }
    cJSON_AddStringToObject(obj->context,"ret","www");
    cJSON_Results_Print(obj,&out);
    //输出的内容
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s", out);
    //将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
    cJSON_Results_free(obj);
    free(out);
}
