#include <application/httpd.h>
#include <SnowFlake.h>
#include <wangyonglin/string.h>
#include <https/client.h>
#include <application/https_successful.h>
#include <Encrypt/Base64.h>
// 解析http头，主要用于get请求时解析uri和请求参数
char *find_http_header(struct evhttp_request *req, struct evkeyvalq *params, const char *query_char)
{
    if (req == NULL || params == NULL || query_char == NULL)
    {
        //    printf("====line:%d,%s\n", __LINE__, "input params is null.");
        return NULL;
    }

    struct evhttp_uri *decoded = NULL;
    char *query = NULL;
    char *query_result = NULL;
    const char *path;
    const char *uri = evhttp_request_get_uri(req); // 获取请求uri

    if (uri == NULL)
    {
        // printf("====line:%d,evhttp_request_get_uri return null\n", __LINE__);
        return NULL;
    }
    // else
    // {
    //  //   printf("====line:%d,Got a GET request for <%s>\n", __LINE__, uri);
    // }

    // 解码uri
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        //   printf("====line:%d,It's not a good URI. Sending BADREQUEST\n", __LINE__);
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }

    // 获取uri中的path部分
    path = evhttp_uri_get_path(decoded);
    if (path == NULL)
    {
        path = "/";
    }
    // else
    // {
    //     printf("====line:%d,path is:%s\n", __LINE__, path);
    // }

    // 获取uri中的参数部分
    query = (char *)evhttp_uri_get_query(decoded);
    if (query == NULL)
    {
        //   printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
        return NULL;
    }

    // 查询指定参数的值
    evhttp_parse_query_str(query, params);
    query_result = (char *)evhttp_find_header(params, query_char);

    return query_result;
}

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

// void RegisterDeviceHandler(struct evhttp_request *request, void *arg)
// {
//     struct _aliutils_apis_t *apis = (struct _aliutils_apis_t *)arg;
//     evhttp_add_header(request->output_headers, "Content-Type", "application/json; charset=UTF-8");
//     evhttp_add_header(request->output_headers, "Connection", "close");
//     struct _https_callback *callback;
//     char *DeviceName = NULL;
//     struct evkeyvalq sign_params = {0};
//     struct evbuffer *evbuffer_body = evbuffer_new();
//     DeviceName = find_http_header(request, &sign_params, "DeviceName"); // 获取get请求uri中的sign参数
//     if (DeviceName)
//     {
//         char *url = NULL;
// //        RegisterDevice(&url, apis, DeviceName);
//         //   message("urls", url);
//         https_client(url, NULL, &callback);
//         if (callback && callback->jsonformat)
//         {
//             evbuffer_add(evbuffer_body, callback->jsonformat, strlen(callback->jsonformat));
//             string_delete(url);
//             goto done;
//         }
//     }
//     char *ostr;
//     https_failure(&ostr, "request uri no param DeviceName");
//     evbuffer_add(evbuffer_body, ostr, strlen(ostr));
//     string_delete(ostr);
//     goto done;
// done:

//     evhttp_send_reply(request, HTTP_OK, "OK", evbuffer_body);
//     if (evbuffer_body)
//         evbuffer_free(evbuffer_body);
//     return;
// }
// void PubHandler(struct evhttp_request *request, void *arg)
// {
//     struct _aliutils_apis_t *apis = (struct _aliutils_apis_t *)arg;
//     evhttp_add_header(request->output_headers, "Content-Type", "application/json; charset=UTF-8");
//     evhttp_add_header(request->output_headers, "Connection", "close");
//     struct _https_callback *callback;
//     char *DeviceName = NULL;
//     char *MessageContentText = NULL;
//     struct evkeyvalq sign_params = {0};
//     struct evbuffer *evbuffer_body = evbuffer_new();
//     DeviceName = find_http_header(request, &sign_params, "DeviceName"); // 获取get请求uri中的sign参数
//     MessageContentText = find_http_header(request, &sign_params, "MessageContentText");
//     int MessageContentTextSize = strlen(MessageContentText);
//     message("DeviceName", DeviceName);
//     message("MessageContentText", MessageContentText);
//     if (DeviceName && MessageContentText)
//     {
//         char *url = NULL;
//         Pub(&url, apis, DeviceName, MessageContentText, MessageContentTextSize);
//         message("url", url);
//         https_client(url, NULL, &callback);
//         if (callback && callback->jsonformat)
//         {
//             evbuffer_add(evbuffer_body, callback->jsonformat, strlen(callback->jsonformat));
//             string_delete(url);
//             goto done;
//         }
//     }
//     char *ostr;
//     https_failure(&ostr, "request uri no param DeviceName");
//     evbuffer_add(evbuffer_body, ostr, strlen(ostr));
//     string_delete(ostr);
//     goto done;
// done:

//     evhttp_send_reply(request, HTTP_OK, "OK", evbuffer_body);
//     if (evbuffer_body)
//         evbuffer_free(evbuffer_body);
//     return;
// }