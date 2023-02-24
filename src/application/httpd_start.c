#include <httpd.h>
#include <curl/curl.h>
#include <cJSON.h>

ok_t httpd_initializing(httpd_t **httpd, app_t *app, char *conf)
{
    if (!app)
    {
        return ArgumentException;
    }
    if (objcrt((void **)httpd, sizeof(httpd_t)) != OK)
    {
        logerr(app->log, "https_server_initializing failed");
        return NullPointerException;
    }
    conf_command arguments[] = {
        {"address", "0.0.0.0", STRING, offsetof(httpd_t, address)},
        {"port",80,INTEGER, offsetof(httpd_t, port)},
        {"timeout_in_secs",15, INTEGER, offsetof(httpd_t, timeout_in_secs)}};

    int arguments_size = sizeof(arguments) / sizeof(arguments[0]);

    if (conf_create((void **)*httpd, conf, NULL, arguments, arguments_size) != OK)
    {
        logerr(app->log, "https_server_initializing conf_mapping   failed");
        return ErrorException;
    }
    logerr(app->log, "address    {%s}", (*httpd)->address);
    logerr(app->log, "port    {%d}", (*httpd)->port);
    logerr(app->log, "timeout_in_secs    {%d}", (*httpd)->timeout_in_secs);
    (*httpd)->app = app;
    return OK;
}

void error_handler(struct evhttp_request *req, void *arg)
{
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = NULL;
    struct evbuffer *evbuffer_body = evbuffer_new();
    printf("Got a GET request for <%s>\n", uri);

    /* Decode the URI */
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        printf("It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON *result = cJSON_CreateObject();
    char *out = NULL;
    if (root && result)
    {
        cJSON_AddFalseToObject(root, "success");
        cJSON_AddStringToObject(root, "reason", "Unknown Error");
        cJSON_AddNumberToObject(root, "errcode", 404);
        cJSON_AddItemToObject(root, "result", result);
        cJSON_AddStringToObject(result, "uri", uri);
        out = cJSON_PrintUnformatted(root);
        if (result)
            cJSON_free(result);
        if (root)
            cJSON_free(root);
    }

    if (out)
    {
        evbuffer_add(evbuffer_body, out, strlen(out));
        evhttp_send_reply(req, 404, "OK", evbuffer_body);
    }
    goto done;
err:
    evhttp_send_error(req, HTTP_NOTFOUND, NULL);
    if (decoded)
        evhttp_uri_free(decoded);

done:
    if (decoded)
        evhttp_uri_free(decoded);
    if (evbuffer_body)
        evbuffer_free(evbuffer_body);
    if (out)
        free(out);
    return;
}

ok_t httpd_start(httpd_t *httpd)
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
        evhttp_set_gencb(httpd->https, error_handler, NULL);
        // 循环监听
        event_dispatch();

        return OK;
    }

    return ErrorException;
}
void httpd_destroy(httpd_t *httpd)
{
    if (httpd->https)
    {
        evhttp_free(httpd->https);
    }
}
