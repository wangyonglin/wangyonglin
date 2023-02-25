#include <application/httpd.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <application/https_successful.h>

httpd_t *httpd = NULL;
struct _command aliyun_commands[] = {{"AccessKeyId", NULL, STRING, offsetof(struct _aliutils_apis_t, AccessKeyId)},
                                     {"AccessKeySecret", NULL, STRING, offsetof(struct _aliutils_apis_t, AccessKeySecret)},
                                     {"ProductKey", "cn-shanghai", STRING, offsetof(struct _aliutils_apis_t, ProductKey)},
                                     {"DeviceName", "JSON", STRING, offsetof(struct _aliutils_apis_t, DeviceName)},
                                     {"Format", "JSON", STRING, offsetof(struct _aliutils_apis_t, Format)},
                                     {"Version", "2020-04-20", STRING, offsetof(struct _aliutils_apis_t, Version)},
                                     {"AccessKeyId", NULL, STRING, offsetof(struct _aliutils_apis_t, AccessKeyId)},
                                     {"SignatureMethod", "HMAC-SHA1", STRING, offsetof(struct _aliutils_apis_t, SignatureMethod)},
                                     {"SignatureVersion", "1.0", STRING, offsetof(struct _aliutils_apis_t, SignatureVersion)},
                                     {"RegionId", "cn-shanghai", STRING, offsetof(struct _aliutils_apis_t, RegionId)},
                                     {"TopicFullName", NULL, STRING, offsetof(struct _aliutils_apis_t, TopicFullName)},
                                     null_command};
struct _aliutils_apis_t *apis;

ok_t httpd_create(app_t *app)
{
    if (!app)
    {
        return ArgumentException;
    }
    if (!objcrt((void **)&httpd, sizeof(struct _httpd_t)))
    {
        logerr(app->log, "httpd_create failed");
        return NullPointerException;
    }
    struct _command commands[] = {
        {"address", "0.0.0.0", STRING, offsetof(struct _httpd_t, address)},
        {"port", (void *)80, INTEGER, offsetof(struct _httpd_t, port)},
        {"timeout_in_secs", 15, INTEGER, offsetof(struct _httpd_t, timeout_in_secs)},
        null_command};

    if (command_init(app, httpd, commands, "HTTPD") != OK)
    {
        return ErrorException;
    }
    logerr(app->log, "address    {%s}", (httpd)->address);
    logerr(app->log, "port    {%d}", (httpd)->port);
    logerr(app->log, "timeout_in_secs    {%d}", (httpd)->timeout_in_secs);
    objcrt(&apis, sizeof(struct _aliutils_apis_t));
    command_init(app, apis, aliyun_commands, "ALIIOT");
    (httpd)->app = app;

    return OK;
}

void error_handler(struct evhttp_request *req, void *arg)
{
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");
    char *ostr;
    https_failure(&ostr, "404 Unknown Error");
    struct evbuffer *evbuffer_body = evbuffer_new();
    evbuffer_add(evbuffer_body, ostr, strlen(ostr));
    evhttp_send_reply(req, 404, "404 Unknown Error", evbuffer_body);
    if (evbuffer_body)
        evbuffer_free(evbuffer_body);
    strdel(ostr);
    return;
}

ok_t httpd_start()
{

    if (httpd)
    {
        // 初始化event API
        event_init();
        // 创建一个HTTPserver
        httpd->https = evhttp_start(httpd->address, httpd->port);
        // 设置超时时间15秒
        evhttp_set_timeout(httpd->https, httpd->timeout_in_secs);

        evhttp_set_cb(httpd->https, "/v1/login", login_handler, httpd);
        evhttp_set_cb(httpd->https, "/v3/pay/transactions/jsapi", v3_pay_transactions_jsapi, httpd);
        evhttp_set_cb(httpd->https, "/wireless", wireless_handler, httpd->app);
        evhttp_set_cb(httpd->https, "/aliyun/pub", PubHandler, apis);
        evhttp_set_cb(httpd->https, "/aliyun/regisgerdevice", RegisterDeviceHandler, apis);
        evhttp_set_gencb(httpd->https, error_handler, NULL);
        // 循环监听
        event_dispatch();

        return OK;
    }

    return ErrorException;
}
void httpd_delete()
{
    if (httpd->https)
    {
        if (apis)
        {
            objdel(apis);
        }
        evhttp_free(httpd->https);
        objdel(httpd);
    }
}
