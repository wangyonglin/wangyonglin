#include <HTTPDHandler.h>
#include <WebServer.h>
#include <SnowFlake.h>
#include <base64.h>
#include <WechatConfig.h>
#include <strings.h>
#include <WechatHttps.h>
#include <aes_256_gcm.h>
#include <ServerResponse.h>
#include <Stringex.h>
#include <ServerResponse.h>

void HTTPDAliyunGetDeviceStatus(struct evhttp_request *request, void *arg)
{
    ServerResponse *response;
    ServerResponseCreate(&response, NULL, request, arg);
    AliyunConfig *aliyunConfig = response->aliConfig;
    cJSON *retResult = NULL;
    Stringex DeviceName = Stringex_null;
    if (ServerResponseQuery(&DeviceName, response, "DeviceName"))
    {
        HTTPSAliyunGetDeviceStatus(&retResult, aliyunConfig, DeviceName.valuestring);
        cJSON *Success = cJSON_GetObjectItem(retResult, "Success");
        if (cJSON_IsTrue(Success))
        {
            cJSON *Data = cJSON_GetObjectItem(retResult, "Data");
            cJSON_DeleteItemFromObject(Data, "Timestamp");
            ServerResponseBySuccessfly(response, Data);
        }
        else
        {
            cJSON *ErrorMessage = cJSON_GetObjectItem(retResult, "ErrorMessage");
            ServerResponseByErrorMessage(response, 400, ErrorMessage->valuestring);
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "未获取请求参数[DeviceName]");
    }
    ServerResponseDelete(response);
    StringexDelete(DeviceName);
    if (retResult)
        cJSON_free(retResult);
}

void HTTPDAliyunRegisterDevice(struct evhttp_request *request, void *arg)
{
    ServerResponse *response;
    ServerResponseCreate(&response, NULL, request, arg);
    AliyunConfig *aliyunConfig = response->aliConfig;
    cJSON *retResult = NULL;
    Stringex DeviceName = Stringex_null;
    if (ServerResponseQuery(&DeviceName, response, "DeviceName"))
    {
        HTTPSAliyunRegisterDevice(&retResult, aliyunConfig, DeviceName.valuestring);
        cJSON *Success = cJSON_GetObjectItem(retResult, "Success");
        if (cJSON_IsTrue(Success))
        {
            cJSON *Data = cJSON_GetObjectItem(retResult, "Data");
            ServerResponseBySuccessfly(response, Data);
        }
        else
        {
            cJSON *ErrorMessage = cJSON_GetObjectItem(retResult, "ErrorMessage");

            ServerResponseByErrorMessage(response, 400, ErrorMessage->valuestring);
        }
    }
    else
    {

        ServerResponseByErrorMessage(response, 400, "未获取请求参数[DeviceName]");
    }
    ServerResponseDelete(response);
    StringexDelete(DeviceName);
    if (retResult)
        cJSON_free(retResult);
}
void HTTPDAliyunUserSet(struct evhttp_request *request, void *arg)
{

    zlog_category_t *log = ((WebServer *)arg)->log;
    AliyunConfig *aliyunConfig = ((WebServer *)arg)->aliConfig;
    ServerResponse *response;
    ServerResponseCreate(&response, log, request, arg);
    cJSON *RetResult = NULL;

    cJSON *postdata = cJSON_Parse(response->body.valuestring);
    if (postdata)
    {

        cJSON *DeviceName = cJSON_GetObjectItem(postdata, "DeviceName");
        if (DeviceName)
        {
            HTTPSAliyunPub(&RetResult, aliyunConfig, DeviceName, postdata);

            cJSON *Success = cJSON_GetObjectItem(RetResult, "Success");
            if (cJSON_IsTrue(Success))
            {
                ServerResponseBySuccessfly(response, postdata);
                postdata = NULL;
            }
            else
            {
                ServerResponseByErrorMessage(response, 400, "未知错误");
            }
        }
        else
        {
            ServerResponseByErrorMessage(response, 400, "未获取请求参数[DeviceName]");
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "未获取POST参数");
    }

    ServerResponseDelete(response);

    if (RetResult)
    {
        cJSON_free(RetResult);
    }

    if (postdata)
    {
        cJSON_free(postdata);
    }
}
