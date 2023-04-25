#include <HTTPDHandler.h>
#include <HTTPDServer.h>
#include <HTTPDResult.h>
#include <SnowFlake.h>

#include <base64.h>

#include <WechatConfig.h>

#include <SHA256WithRSA.h>
#include <strings.h>
#include <WechatHttps.h>
#include <string_by_timestamp.h>
#include <string_by_hex.h>
#include <aes_256_gcm.h>
#include <ResultUtil.h>
#include <ServerResponse.h>
#include <Stringex.h>

// 解析http头，主要用于get请求时解析uri和请求参数
// char *find_http_header(struct evhttp_request *req, struct evkeyvalq *params, const char *query_char)
// {
//     if (req == NULL || params == NULL || query_char == NULL)
//     {
//         printf("====line:%d,%s\n", __LINE__, "input params is null.");
//         return NULL;
//     }

//     struct evhttp_uri *decoded = NULL;
//     char *query = NULL;
//     char *query_result = NULL;
//     const char *path;
//     const char *uri = evhttp_request_get_uri(req); // 获取请求uri

//     if (uri == NULL)
//     {
//         printf("====line:%d,evhttp_request_get_uri return null\n", __LINE__);
//         return NULL;
//     }
//     else
//     {
//         printf("====line:%d,Got a GET request for <%s>\n", __LINE__, uri);
//     }

//     // 解码uri
//     decoded = evhttp_uri_parse(uri);
//     if (!decoded)
//     {
//         printf("====line:%d,It's not a good URI. Sending BADREQUEST\n", __LINE__);
//         evhttp_send_error(req, HTTP_BADREQUEST, 0);
//         return NULL;
//     }

//     // 获取uri中的path部分
//     path = evhttp_uri_get_path(decoded);
//     if (path == NULL)
//     {
//         path = "/";
//     }
//     else
//     {
//         printf("====line:%d,path is:%s\n", __LINE__, path);
//     }

//     // 获取uri中的参数部分
//     query = (char *)evhttp_uri_get_query(decoded);
//     if (query == NULL)
//     {
//         printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
//         return NULL;
//     }

//     // 查询指定参数的值
//     evhttp_parse_query_str(query, params);
//     query_result = (char *)evhttp_find_header(params, query_char);

//     return query_result;
// }

void HTTPDAliyunGetDeviceStatus(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
    httpd_result_t *result;
    httpd_result_create(&result, request, arg);
    string_by_t jsonstring = string_null_command;
    ResultUtil *ResUtil = NULL;
    ResultUtilCreate(&ResUtil);
    cJSON *retResult = NULL;
    char *DeviceName = NULL;
    DeviceName = httpd_result_get_params(result, "DeviceName");
    if (DeviceName)
    {
        HTTPSAliyunGetDeviceStatus(&retResult, httpd->aliConfig, DeviceName);
        cJSON *Success = cJSON_GetObjectItem(retResult, "Success");
        if (cJSON_IsTrue(Success))
        {
            cJSON *Data = cJSON_GetObjectItem(retResult, "Data");
            cJSON_DeleteItemFromObject(Data, "Timestamp");
            ResultUtilSuccessful(ResUtil, Data);
        }
        else
        {
            cJSON *ErrorMessage = cJSON_GetObjectItem(retResult, "ErrorMessage");
            ResultUtilFailure(ResUtil, 400, ErrorMessage->valuestring);
        }
    }
    else
    {

        ResultUtilFailure(ResUtil, 400, "未获取请求参数[DeviceName]");
    }
    httpd_result_complete(result, ResUtil);

    ResultUtilDelete(ResUtil);
    httpd_result_delete(result);
    string_delete(jsonstring);
    if (retResult)
        cJSON_free(retResult);
}

void HTTPDAliyunRegisterDevice(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
    httpd_result_t *result;
    httpd_result_create(&result, request, arg);
    ResultUtil *ResUtil = NULL;
    ResultUtilCreate(&ResUtil);
    cJSON *retResult = NULL;
    char *DeviceName = NULL;
    DeviceName = httpd_result_get_params(result, "DeviceName");
    if (DeviceName)
    {
        HTTPSAliyunRegisterDevice(&retResult, httpd->aliConfig, DeviceName);
        cJSON *Success = cJSON_GetObjectItem(retResult, "Success");
        if (cJSON_IsTrue(Success))
        {
            cJSON *Data = cJSON_GetObjectItem(retResult, "Data");
            ResultUtilSuccessful(ResUtil, Data);
        }
        else
        {
            cJSON *ErrorMessage = cJSON_GetObjectItem(retResult, "ErrorMessage");

            ResultUtilFailure(ResUtil, 400, ErrorMessage->valuestring);
        }
    }
    else
    {

        ResultUtilFailure(ResUtil, 400, "未获取请求参数[DeviceName]");
    }
    httpd_result_complete(result, ResUtil);
    ResultUtilDelete(ResUtil);
    httpd_result_delete(result);
    if (retResult)
        cJSON_free(retResult);
}
void HTTPDAliyunUserSet(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
    zlog_category_t *log =((HTTPDServer *)arg)->log;
    ServerResponse *response;
    ServerResponseCreate(&response,log, request, arg);
    cJSON *RetResult = NULL;
    ResultUtil *ResUtil = NULL;
    ResultUtilCreate(&ResUtil);
    cJSON *postdata = cJSON_Parse(response->body.valuestring);
    if (postdata)
    {

        cJSON *DeviceName = cJSON_GetObjectItem(postdata, "DeviceName");
        if (DeviceName)
        {
            HTTPSAliyunPub(&RetResult, httpd->aliConfig, DeviceName, postdata);

            cJSON *Success = cJSON_GetObjectItem(RetResult, "Success");
            if (cJSON_IsTrue(Success))
                ResultUtilSuccessful(ResUtil, postdata);
            else
                ResultUtilComplete(ResUtil, 400, postdata, "未知错误");
        }
        else
        {

            ResultUtilFailure(ResUtil, 400, "未获取请求参数[DeviceName]");
        }
    }
    else
    {

        ResultUtilFailure(ResUtil, 400, "未获取POST参数");
    }

    ServerResponseResultUtilComplete(response, ResUtil);

    ServerResponseDelete(response);
    ResultUtilDelete(ResUtil);
    if (RetResult)
        cJSON_free(RetResult);
    cJSON_free(postdata);
}
