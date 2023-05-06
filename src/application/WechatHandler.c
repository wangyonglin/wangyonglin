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

    zlog_category_t *log = ((WebServer *)arg)->log;
    WechatConfig *wechatConfig = ((WebServer *)arg)->wConfig;

    ServerResponse *response;
    ServerResponseCreate(&response, log, request, arg);
    WechatHttpsCallback callback;
    cJSON *retResult = NULL;
    cJSON *retResultItem = NULL;
    char urlString[512] = {0};
    Stringex js_code;

    if (ServerResponseQuery(&js_code, response, "js_code"))
    {
        sprintf(urlString,
                "https://api.weixin.qq.com/sns/jscode2session?appid=%s&secret=%s&js_code=%s&grant_type=authorization_code",
                wechatConfig->appid.valuestring,
                wechatConfig->secret.valuestring,
                js_code.valuestring);
        WechatHttpsJscode2session(urlString, &callback);
        if (callback.memory && callback.size > 2)
        {
            retResult = cJSON_Parse(callback.memory);

            if (retResult)
            {

                if (retResultItem = cJSON_GetObjectItem(retResult, "errmsg"))
                {

                    ServerResponseByErrorMessage(response, 400, retResultItem->valuestring);
                }
                else if (retResultItem = cJSON_GetObjectItem(retResult, "openid"))
                {
                    cJSON_DeleteItemFromObject(retResult, "session_key");
                    cJSON_AddStringToObject(retResult, "payer_client_ip", response->request->remote_host);

                    ServerResponseBySuccessfly(response, retResult);
                }
                else
                {
                    ServerResponseByErrorMessage(response, 400, "服务返回数据解析失败");
                }
            }
            free(callback.memory);
        }
        else
        {
            ServerResponseByErrorMessage(response, 400, "返回值解析失败");
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "未获取请求参数[js_code]");
    }
    StringexDelete(js_code);

    ServerResponseDelete(response);

    if (retResultItem)
        cJSON_free(retResultItem);
    if (retResult)
        cJSON_free(retResult);
}
