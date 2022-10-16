#include <https_server.h>
#include <system/string.h>

void login_handler(struct evhttp_request *req, void *arg)
{
    //获取POST方法的数据
    size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
    char request_data[1024] = {0};
    char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);
    cJSON *root = NULL;
    if (post_size > 0 && post_size < 1024)
    {
        memcpy(request_data, post_data, post_size);
        root = cJSON_Parse(request_data);
    }
    //给前段回复一个响应结果
    cJSON *response_root = cJSON_CreateObject();
    if (root && response_root)
    {
        cJSON *username_obj = cJSON_GetObjectItem(root, "username");
        cJSON *password_obj = cJSON_GetObjectItem(root, "password");
        cJSON *version_obj = cJSON_GetObjectItem(root, "version");

        if (username_obj && password_obj && version_obj)
        {
            cJSON_AddStringToObject(response_root, "username", username_obj->valuestring);
            cJSON_AddStringToObject(response_root, "password", password_obj->valuestring);
            cJSON_AddStringToObject(response_root, "version", version_obj->valuestring);
        }
    }
    cJSON_AddNumberToObject(response_root, "ret", 0);
    char *response_str = cJSON_Print(response_root);
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");

    //输出的内容
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s", response_str);

    //将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, HTTP_OK, "OK", buf);

    evbuffer_free(buf);

    cJSON_Delete(response_root);
    free(response_str);
}

https_httpd_config_t *httpd_initializing(https_httpd_config_t **httpd, const char *address, uint16_t port)
{
    if (allocate_object((void **)httpd, sizeof(https_httpd_config_t)))
    {
        //初始化event API
        event_init();
        //创建一个HTTPserver
        (*httpd)->httpd = evhttp_start(address, port);
        //设置超时时间15秒
        evhttp_set_timeout((*httpd)->httpd, 15);
        evhttp_set_cb((*httpd)->httpd, "/v1/login", login_handler, NULL);
        //循环监听
        event_dispatch();
        return (*httpd);
    }
    return NULL;
}
void httpd_cleanup(https_httpd_config_t *httpd)
{
    if (httpd)
    {
        evhttp_free(httpd->httpd);
    }
}
