#include <HTTPDHandler.h>
#include <HTTPDServer.h>
#include <HTTPDResult.h>
#include <SnowFlake.h>

#include <Encrypt/Base64.h>

#include <WechatPayment.h>
#include <WechatHttps.h>
#include <Encrypt/SHA256WithRSA.h>
#include <strings.h>
#include <WechatHttps.h>
#include <string_by_timestamp.h>
#include <string_by_hex.h>

// 解析http头，主要用于get请求时解析uri和请求参数
char *find_http_header(struct evhttp_request *req, struct evkeyvalq *params, const char *query_char)
{
    if (req == NULL || params == NULL || query_char == NULL)
    {
        printf("====line:%d,%s\n", __LINE__, "input params is null.");
        return NULL;
    }

    struct evhttp_uri *decoded = NULL;
    char *query = NULL;
    char *query_result = NULL;
    const char *path;
    const char *uri = evhttp_request_get_uri(req); // 获取请求uri

    if (uri == NULL)
    {
        printf("====line:%d,evhttp_request_get_uri return null\n", __LINE__);
        return NULL;
    }
    else
    {
        printf("====line:%d,Got a GET request for <%s>\n", __LINE__, uri);
    }

    // 解码uri
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        printf("====line:%d,It's not a good URI. Sending BADREQUEST\n", __LINE__);
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return NULL;
    }

    // 获取uri中的path部分
    path = evhttp_uri_get_path(decoded);
    if (path == NULL)
    {
        path = "/";
    }
    else
    {
        printf("====line:%d,path is:%s\n", __LINE__, path);
    }

    // 获取uri中的参数部分
    query = (char *)evhttp_uri_get_query(decoded);
    if (query == NULL)
    {
        printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
        return NULL;
    }

    // 查询指定参数的值
    evhttp_parse_query_str(query, params);
    query_result = (char *)evhttp_find_header(params, query_char);

    return query_result;
}

// void login_handler(struct evhttp_request *req, void *arg)
// {
//     // 获取POST方法的数据
//     size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
//     char request_data[1024] = {0};
//     char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);
//     cJSON *root = NULL;
//     if (post_size > 0 && post_size < 1024)
//     {
//         memcpy(request_data, post_data, post_size);
//         root = cJSON_Parse(request_data);
//     }
//     // 给前段回复一个响应结果
//     cJSON *response_root = cJSON_CreateObject();
//     if (root && response_root)
//     {
//         cJSON *username_obj = cJSON_GetObjectItem(root, "username");
//         cJSON *password_obj = cJSON_GetObjectItem(root, "password");
//         cJSON *version_obj = cJSON_GetObjectItem(root, "version");

//         if (username_obj && password_obj && version_obj)
//         {
//             cJSON_AddStringToObject(response_root, "username", username_obj->valuestring);
//             cJSON_AddStringToObject(response_root, "password", password_obj->valuestring);
//             cJSON_AddStringToObject(response_root, "version", version_obj->valuestring);
//         }
//     }
//     cJSON_AddNumberToObject(response_root, "ret", 0);
//     char *response_str = cJSON_Print(response_root);
//     evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
//     evhttp_add_header(req->output_headers, "Connection", "close");

//     // 输出的内容
//     struct evbuffer *buf = evbuffer_new();
//     evbuffer_add_printf(buf, "%s", response_str);

//     // 将封装好的evbuffer 发送给客户端
//     evhttp_send_reply(req, HTTP_OK, "OK", buf);

//     evbuffer_free(buf);

//     cJSON_Delete(response_root);
//     free(response_str);
// }

// void v3_pay_transactions_jsapi(struct evhttp_request *request, void *arg)
// {
//     httpd_t *httpd = (httpd_t *)arg;

//     evhttp_add_header(request->output_headers, "Content-Type", "application/json; charset=UTF-8");
//     evhttp_add_header(request->output_headers, "Connection", "close");
//     cJSON *root = NULL;
//     cJSON *amount = NULL;
//     cJSON *payer = NULL;
//     // 获取POST方法的数据
//     size_t post_size = EVBUFFER_LENGTH(request->input_buffer);
//     char request_data[1024] = {0};
//     char *post_data = (char *)EVBUFFER_DATA(request->input_buffer);
//     if (post_size > 0 && post_size < 1024)
//     {
//         memcpy(request_data, post_data, post_size);
//         root = cJSON_Parse(request_data);
//     }

//     char id[32] = {0};
//     sprintf(id, "%llu", SnowFlake_IdGenerator());
//     // 给前段回复一个响应结果
//     cJSON *json_response = cJSON_CreateObject();
//     if (root && json_response)
//     {
//         cJSON *request_json_openid = cJSON_GetObjectItem(root, "openid");
//         cJSON *request_json_name = cJSON_GetObjectItem(root, "name");
//         cJSON *request_json_total = cJSON_GetObjectItem(root, "total");
//         if (request_json_openid && request_json_total)
//         {
//             cJSON_AddStringToObject(json_response, "appid", "wxa054e8196d3492e3");
//             cJSON_AddStringToObject(json_response, "mchid", "1614902012");
//             cJSON_AddStringToObject(json_response, "description", id);
//             cJSON_AddStringToObject(json_response, "out_trade_no", id);
//             cJSON_AddStringToObject(json_response, "notify_url", "https://www.weixin.qq.com/wxpay/pay.php");
//             cJSON_AddItemToObject(json_response, "amount", amount = cJSON_CreateObject());
//             cJSON_AddStringToObject(amount, "currency", "CNY");
//             cJSON_AddNumberToObject(amount, "total", request_json_total->valueint);
//             cJSON_AddItemToObject(json_response, "payer", payer = cJSON_CreateObject());
//             cJSON_AddStringToObject(payer, "openid", request_json_openid->valuestring);
//         }
//     }

//     char *string_response = cJSON_PrintUnformatted(json_response);

//     // 输出的内容
//     struct evbuffer *buffer = evbuffer_new();
//     evbuffer_add_printf(buffer, "%s", string_response);
//     // 将封装好的evbuffer 发送给客户端
//     evhttp_send_reply(request, HTTP_OK, "OK", buffer);
//     evbuffer_free(buffer);
//     cJSON_Delete(root);
//     cJSON_Delete(json_response);
//     free(string_response);
// }

// void jsapibuyHandler(struct evhttp_request *request, void *arg)
// {
//     httpd_t *httpd = (httpd_t *)arg;
//     httpd_result_t *result;
//     httpd_result_create(&result, httpd->log, request, arg);
//     string_t jsonstring = string_null_command;
//     char *DeviceName = NULL;
//     DeviceName = httpd_result_get_params(result, "DeviceName");
//     if (DeviceName)
//     {
//         jsonstring = aliapis_https_register_device(result->httpd->aliapis, DeviceName);
//         httpd_result_successful(result, jsonstring);
//     }
//     else
//     {
//         httpd_result_failure(result, "request uri no param DeviceName");
//     }

//     httpd_result_delete(result);
//     string_delete(jsonstring);
// }

void HTTPDAliyunRegisterDevice(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
    httpd_result_t *result;
    httpd_result_create(&result, httpd->log, request, arg);
    string_by_t jsonstring = string_null_command;
    char *DeviceName = NULL;
    DeviceName = httpd_result_get_params(result, "DeviceName");
    if (DeviceName)
    {
        aliapis_https_register_device(&jsonstring, httpd->aliConfig, DeviceName);
        httpd_result_successful(result, jsonstring);
    }
    else
    {
        httpd_result_failure(result, HTTPD_INVALID_PARAMETE, "request uri no param DeviceName");
    }

    httpd_result_delete(result);
    string_delete(jsonstring);
}

// void PublishHandler(struct evhttp_request *request, void *arg)
// {
//     httpd_t *httpd = (httpd_t *)arg;
//     string_t jsonstring = string_null_command;
//     httpd_result_t *result;
//     httpd_result_create(&result, httpd->log, request, arg);
//     char *DeviceName = NULL;
//     char *MessageContentText = NULL;
//     DeviceName = httpd_result_get_params(result, "DeviceName");
//     MessageContentText = httpd_result_get_params(result, "MessageContentText");
//     if (DeviceName && MessageContentText)
//     {
//         jsonstring = aliapis_https_publish(result->httpd->aliapis, DeviceName, MessageContentText, strlen(MessageContentText));
//         httpd_result_successful(result, jsonstring);
//     }
//     else
//     {
//         httpd_result_failure(result, "参数不能为空");
//     }
//     httpd_result_delete(result);
//     string_delete(jsonstring);
// }
/**
 * @brief 微信支付方法
 *
 */
void HTTPDrWechatPurchase(struct evhttp_request *request, void *arg)
{
    char tmpString[128] = {0};
    HTTPDServer *httpd = (HTTPDServer *)arg;
    string_by_t jsonstring = string_null_command;
    httpd_result_t *result;
    httpd_result_create(&result, httpd->log, request, arg);
    string_by_t authorization = string_null_command;
    string_by_t url = string_null_command;
    string_by_t body = string_null_command;
    char *openIdString = NULL;
    integer_by_t totalInteger = 0;
    const char *urlstring = "/v3/pay/transactions/jsapi";

    openIdString = httpd_result_get_params(result, "OpenId");
    char *totalString = httpd_result_get_params(result, "Total");
    if (totalString)
    {
       totalInteger= atoi(totalString);
    }
    if (openIdString)
    {
        string_by_t timeStamp = string_null_command;
        string_by_timestamp(&timeStamp);

        string_by_t nonceStr = string_null_command;
        string_by_hex(&nonceStr, 32);

        string_create(&url, (char *)urlstring, strlen(urlstring));
        WechatPaymentPurchaseAuthorization(httpd->payment, openIdString,totalInteger, url, &body, &authorization);
        WechatHttpsCallback httpsCallback;
        WechatHttpsPost(httpd->payment, body, authorization, url, &httpsCallback);
        cJSON *parseJSONString = NULL;
        cJSON *rootJSONString = cJSON_CreateObject();
        if ((parseJSONString = cJSON_Parse(httpsCallback.memory)) && rootJSONString)
        {
            cJSON *prepay_id = cJSON_GetObjectItem(parseJSONString, "prepay_id");
            if (prepay_id)
            {
                char prepay_idString[64] = {0};
                sprintf(prepay_idString, "prepay_id=%s", prepay_id->valuestring);
                bzero(tmpString, sizeof(tmpString));
                strcat(tmpString, httpd->payment->appid.valuestring);
                strcat(tmpString, "\n");
                strcat(tmpString, timeStamp.valuestring);
                strcat(tmpString, "\n");
                strcat(tmpString, nonceStr.valuestring);
                strcat(tmpString, "\n");
                strcat(tmpString, "prepay_id=");
                strcat(tmpString, prepay_id->valuestring);
                strcat(tmpString, "\n");
                char *paySign;
                Sha256WithRSASignatureEx(httpd->payment->apiclient_key, tmpString, strlen(tmpString), &paySign);

                cJSON_AddStringToObject(rootJSONString, "package", prepay_idString);
                cJSON_AddStringToObject(rootJSONString, "timeStamp", timeStamp.valuestring);
                cJSON_AddStringToObject(rootJSONString, "nonceStr", nonceStr.valuestring);
                cJSON_AddStringToObject(rootJSONString, "signType", "RSA");
                cJSON_AddStringToObject(rootJSONString, "paySign", paySign);
                char *_jsonString = cJSON_PrintUnformatted(rootJSONString);
                if (_jsonString)
                {
                    string_create(&jsonstring, _jsonString, strlen(_jsonString));
                    free(paySign);
                    free(_jsonString);
                }
                cJSON_free(rootJSONString);
                cJSON_free(parseJSONString);
            }
            else
            {
                string_create(&jsonstring, httpsCallback.memory, httpsCallback.size);
            }

            httpd_result_complete(result, 400, jsonstring);
        }
        else
        {
            httpd_result_failure(result, HTTPD_INVALID_PARAMETE, "unknown error");
        }

        string_delete(authorization);
        string_delete(jsonstring);
        string_delete(url);
        if (httpsCallback.memory)
        {
            free(httpsCallback.memory);
        }
        string_delete(timeStamp);
        string_delete(nonceStr);
    }
    else
    {
        httpd_result_failure(result, HTTPD_INVALID_PARAMETE, "参数OpenId不能为空");
    }

    httpd_result_delete(result);
}

void error_handler(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;

    httpd_result_t *result;
    httpd_result_create(&result, httpd->log, request, arg);
    httpd_result_failure(result, 404, "404 not found");
    httpd_result_delete(result);
}

void HTTPDrWechatJscode2session(struct evhttp_request *request, void *arg)
{

    HTTPDServer *httpd = (HTTPDServer *)arg;
    httpd_result_t *result;
    httpd_result_create(&result, httpd->log, request, arg);
    string_by_t jsonstring = string_null_command;
    WechatHttpsCallback callback;
    char urlString[512] = {0};

    char *js_code = httpd_result_get_params(result, "js_code");
    if (js_code)
    {
        sprintf(urlString, "https://api.weixin.qq.com/sns/jscode2session?appid=%s&secret=%s&js_code=%s&grant_type=authorization_code", httpd->payment->appid.valuestring, httpd->payment->secret.valuestring, js_code);
        WechatHttpsJscode2session(urlString, &callback);
        string_create(&jsonstring, callback.memory, callback.size);
        httpd_result_successful(result, jsonstring);
        string_delete(jsonstring);
        if (callback.memory)
        {
            free(callback.memory);
        }
    }
    else
    {
        httpd_result_failure(result, HTTPD_INVALID_PARAMETE, "request uri no param js_code");
    }
    httpd_result_delete(result);
}
void HTTPDrWechatNotifyUrl(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
    string_by_t jsonstring = string_null_command;
    httpd_result_t *result;
    httpd_result_create(&result, httpd->log, request, arg);
    string_create(&jsonstring, "wangyonglin", 11);
    httpd_result_successful(result, jsonstring);
    httpd_result_delete(result);
    string_delete(jsonstring);
}