#include <httpd.h>
#include <SnowFlake.h>

void login_handler(struct evhttp_request *req, void *arg)
{
    // 获取POST方法的数据
    size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
    char request_data[1024] = {0};
    char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);
    cJSON *root = NULL;
    if (post_size > 0 && post_size < 1024)
    {
        memcpy(request_data, post_data, post_size);
        root = cJSON_Parse(request_data);
    }
    // 给前段回复一个响应结果
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

    // 输出的内容
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s", response_str);

    // 将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, HTTP_OK, "OK", buf);

    evbuffer_free(buf);

    cJSON_Delete(response_root);
    free(response_str);
}

void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg)
{
    httpd_t *httpd = (httpd_t *)arg;
    
    evhttp_add_header(request->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(request->output_headers, "Connection", "close");
    cJSON *root = NULL;
    cJSON *amount = NULL;
    cJSON *payer = NULL;
    // 获取POST方法的数据
    size_t post_size = EVBUFFER_LENGTH(request->input_buffer);
    char request_data[1024] = {0};
    char *post_data = (char *)EVBUFFER_DATA(request->input_buffer);
    if (post_size > 0 && post_size < 1024)
    {
        memcpy(request_data, post_data, post_size);
        root = cJSON_Parse(request_data);
    }

    char id[32] = {0};
    sprintf(id, "%llu", SnowFlake_IdGenerator());
    // 给前段回复一个响应结果
    cJSON *json_response = cJSON_CreateObject();
    if (root && json_response)
    {
        cJSON *request_json_openid = cJSON_GetObjectItem(root, "openid");
        cJSON *request_json_name = cJSON_GetObjectItem(root, "name");
        cJSON *request_json_total = cJSON_GetObjectItem(root, "total");
        if (request_json_openid && request_json_total)
        {
            cJSON_AddStringToObject(json_response, "appid", "wxa054e8196d3492e3");
            cJSON_AddStringToObject(json_response, "mchid", "1614902012");
            cJSON_AddStringToObject(json_response, "description", id);
            cJSON_AddStringToObject(json_response, "out_trade_no", id);
            cJSON_AddStringToObject(json_response, "notify_url", "https://www.weixin.qq.com/wxpay/pay.php");
            cJSON_AddItemToObject(json_response, "amount", amount = cJSON_CreateObject());
            cJSON_AddStringToObject(amount, "currency", "CNY");
            cJSON_AddNumberToObject(amount, "total", request_json_total->valueint);
            cJSON_AddItemToObject(json_response, "payer", payer = cJSON_CreateObject());
            cJSON_AddStringToObject(payer, "openid", request_json_openid->valuestring);
        }
    }

    char *string_response = cJSON_PrintUnformatted(json_response);

    // 输出的内容
    struct evbuffer *buffer = evbuffer_new();
    evbuffer_add_printf(buffer, "%s", string_response);
    // 将封装好的evbuffer 发送给客户端
    evhttp_send_reply(request, HTTP_OK, "OK", buffer);
    evbuffer_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(json_response);
    free(string_response);
}

void wireless_handler(struct evhttp_request *request, void *arg)
{
    app_t *app = (app_t *)arg;
    evhttp_add_header(request->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(request->output_headers, "Connection", "close");

    struct evkeyvalq *headers;
    struct evkeyval *header;

    cJSON *root = cJSON_CreateObject();
    headers = evhttp_request_get_input_headers(request);
    for (header = headers->tqh_first; header;
         header = header->next.tqe_next)
    {
        if ((strcmp(header->key, "topic") == 0) || (strcmp(header->key, "TOPIC") == 0))
        {
            cJSON_AddStringToObject(root, header->key, header->value);
        }
        else if ((strcmp(header->key, "payload") == 0) || (strcmp(header->key, "PAYLOAD") == 0))
        {
            cJSON_AddStringToObject(root, header->key, header->value);
        }

        //   printf("  %s: %s\n", header->key, header->value);
    }
    // 输出的内容
    char *out = cJSON_PrintUnformatted(root);

    if (sutpc_sndMsgQue(app->msg1, 100, out, strlen(out)) != OK)
    {
        printf("\tdata%s err:%s\n", out, strerror(errno));
    }
    struct evbuffer *buffer = evbuffer_new();
    evbuffer_add(buffer, out, strlen(out));
    // 将封装好的evbuffer 发送给客户端
    evhttp_send_reply(request, HTTP_OK, "OK", buffer);
    evbuffer_free(buffer);
    free(out);
    cJSON_Delete(root);
    return;
}