#include <WechatHandler.h>
#include <WechatConfig.h>
#include <UserMain.h>
#include <StringexBase64.h>
#include <ServerResponse.h>
#include <StringexSha256WithRsa.h>
#include <HTTPSUtils.h>

char *urlpathstring = "/v3/pay/transactions/jsapi";


void WechatHandlerOpenId(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
        zlog_category_t *log =((HTTPDServer *)arg)->log;
    ServerResponse *response;
    ServerResponseCreate(&response,log, request, arg);

    WechatHttpsCallback callback;
    ResultUtil *ResUtil = NULL;
    ResultUtilCreate(&ResUtil);
    cJSON *retResult = NULL;
    cJSON *retResultItem = NULL;
    char urlString[512] = {0};
    Stringex js_code;

    if (ServerResponseQuery(&js_code, response, "js_code"))
    {
        sprintf(urlString, "https://api.weixin.qq.com/sns/jscode2session?appid=%s&secret=%s&js_code=%s&grant_type=authorization_code", httpd->wConfig->appid.valuestring, httpd->wConfig->secret.valuestring, js_code.valuestring);
        WechatHttpsJscode2session(urlString, &callback);
        if (callback.memory && callback.size > 2)
        {
            retResult = cJSON_Parse(callback.memory);

            if (retResult)
            {

                if (retResultItem = cJSON_GetObjectItem(retResult, "errmsg"))
                {

                    ResultUtilFailure(ResUtil, 400, retResultItem->valuestring);
                }
                else if (retResultItem = cJSON_GetObjectItem(retResult, "openid"))
                {
                    cJSON_DeleteItemFromObject(retResult, "session_key");
                    cJSON_AddStringToObject(retResult, "payer_client_ip", response->request->remote_host);
                    ResultUtilSuccessful(ResUtil, retResult);
                }
                else
                {

                    ResultUtilFailure(ResUtil, 400, "服务返回数据解析失败");
                }
            }
            free(callback.memory);
        }
        else
        {

            ResultUtilFailure(ResUtil, 400, "返回值解析失败");
        }
    }
    else
    {

        ResultUtilFailure(ResUtil, 400, "未获取请求参数[js_code]");
    }
    StringexDelete(js_code);
    ServerResponseResultUtilComplete(response, ResUtil);
    ServerResponseDelete(response);
    ResultUtilDelete(ResUtil);
    if (retResultItem)
        cJSON_free(retResultItem);
    if (retResult)
        cJSON_free(retResult);
}

