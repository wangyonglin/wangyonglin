#include <Stringex.h>
#include <WechatConfig.h>
#include <HTTPDServer.h>
#include <ServerResponse.h>
#include <HTTPSUtils.h>
#include <StringexSha256WithRsa.h>
#include <WechatConfigAuthorization.h>
#include <WechatHandlerShipping.h>

Boolean WechatHandlerShippingBody(Stringex *body, ServerResponse *response, Stringex appid, Stringex mchid, Stringex notify_url)
{
    Boolean retvalue = Boolean_false;
    if (!Stringex_IsString(response->body) && !Stringex_IsString(appid) && !Stringex_IsString(mchid) && !Stringex_IsString(notify_url))
    {
        return retvalue;
    }

    cJSON *openid = NULL;
    cJSON *total = NULL;
    cJSON *description = NULL;
    cJSON *device_id = NULL;
    Stringex id = Stringex_null;

    cJSON *tojsonBody = cJSON_Parse(response->body.valuestring);
    if (tojsonBody)
    {
        openid = cJSON_GetObjectItem(tojsonBody, "openid");
        total = cJSON_GetObjectItem(tojsonBody, "total");
        description = cJSON_GetObjectItem(tojsonBody, "description");
        device_id = cJSON_GetObjectItem(tojsonBody, "device_id");
        if (openid && total && description && device_id)
        {
            cJSON *shipping = cJSON_CreateObject();
            cJSON *amount = cJSON_CreateObject();
            cJSON *payer = cJSON_CreateObject();
            cJSON *scene_info = cJSON_CreateObject();
            if (shipping && amount && payer && scene_info)
            {

                StringexId(&id);
                cJSON_AddStringToObject(shipping, "appid", appid.valuestring);
                cJSON_AddStringToObject(shipping, "mchid", mchid.valuestring);
                cJSON_AddStringToObject(shipping, "description", description->valuestring);
                cJSON_AddStringToObject(shipping, "out_trade_no", id.valuestring);
                cJSON_AddStringToObject(shipping, "notify_url", notify_url.valuestring);
                cJSON_AddItemToObject(shipping, "amount", amount);
                cJSON_AddStringToObject(amount, "currency", "CNY");
                cJSON_AddNumberToObject(amount, "total", total->valueint);
                cJSON_AddItemToObject(shipping, "payer", payer);
                cJSON_AddStringToObject(payer, "openid", openid->valuestring);
                cJSON_AddItemToObject(shipping, "scene_info", scene_info);
                cJSON_AddStringToObject(scene_info, "payer_client_ip", response->request->remote_host);
                cJSON_AddStringToObject(scene_info, "device_id", device_id->valuestring);
                char *out = cJSON_PrintUnformatted(shipping);
                if (out)
                {
                    size_t outlength = strlen(out);
                    StringexCreate(body, out, outlength);
                    retvalue = Boolean_true;
                    free(out);
                }
                StringexDelete(id);
            }
            cJSON_free(shipping);
            cJSON_free(amount);
            cJSON_free(payer);
            cJSON_free(scene_info);
        }
        cJSON_Delete(tojsonBody);
    }
    return retvalue;
}

Boolean WechatHandlerShippingResult(
    ServerResponse *response,
    Stringex apiclient_key,
    Stringex appid,
    char *jsonback)
{
    Boolean err = Boolean_false;
    if (!Stringex_IsString(apiclient_key) && !Stringex_IsEmpty(apiclient_key))
    {
        ServerResponseByErrorMessage(response, 400, "apiclient_key");
        return err;
    }

    if (!Stringex_IsString(appid) && !Stringex_IsEmpty(appid))
    {
        ServerResponseByErrorMessage(response, 400, "appid");
        return err;
    }

    if (!jsonback)
    {
        ServerResponseByErrorMessage(response, 400, "服务商返回值为空");
        return err;
    }

    cJSON *parse = NULL;

    if ((parse = cJSON_Parse(jsonback)))
    {
        Stringex paySign = Stringex_null;
        Stringex timeStamp = Stringex_null;
        Stringex nonceStr = Stringex_null;

        StringexNonce(&nonceStr, 32);
        StringexTimeUTC(&timeStamp);

        cJSON *prepay_id = cJSON_GetObjectItemCaseSensitive(parse, "prepay_id");
        if (cJSON_IsString(prepay_id) && (prepay_id->valuestring != NULL))
        {
            char prepay_idString[64] = {0};
            strcat(prepay_idString,"prepay_id=");
            strcat(prepay_idString,prepay_id->valuestring);
            char plainText[512] = {0};
            strcat(plainText, appid.valuestring);
            strcat(plainText, "\n");
            strcat(plainText, timeStamp.valuestring);
            strcat(plainText, "\n");
            strcat(plainText, nonceStr.valuestring);
            strcat(plainText, "\n");
            strcat(plainText, "prepay_id=");
            strcat(plainText, prepay_id->valuestring);
            strcat(plainText, "\n");

            Stringex_Sha256WithRSA_Signature(&paySign, apiclient_key, plainText);
            cJSON *root = cJSON_CreateObject();
            if (root)
            {
                cJSON_AddStringToObject(root, "package", prepay_idString);
                cJSON_AddStringToObject(root, "timeStamp", timeStamp.valuestring);
                cJSON_AddStringToObject(root, "nonceStr", nonceStr.valuestring);
                cJSON_AddStringToObject(root, "signType", "RSA");
                cJSON_AddStringToObject(root, "paySign", paySign.valuestring);
                ServerResponseBySuccessfly(response, root);
                StringexDelete(paySign);
                StringexDelete(timeStamp);
                StringexDelete(nonceStr);
                err = Boolean_true;
            }
        }
        else
        {

            cJSON *message = cJSON_GetObjectItemCaseSensitive(parse, "message");
            if (cJSON_IsString(message) && (message->valuestring != NULL))
            {
                ServerResponseByErrorMessage(response, 400, message->valuestring);
            }
            else
            {
                ServerResponseByErrorMessage(response, 400, "未知错误");
            }
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "解析返回数据失败");
    }
    return err;
}
void WechatHandlerShipping(struct evhttp_request *request, void *arg)
{

    Stringex appid = ((HTTPDServer *)arg)->wConfig->appid;
    Stringex mchid = ((HTTPDServer *)arg)->wConfig->mchid;
    Stringex notify_url = ((HTTPDServer *)arg)->wConfig->notify_url;
    Stringex apiclient_key = ((HTTPDServer *)arg)->wConfig->apiclient_key;
    Stringex serial_no = ((HTTPDServer *)arg)->wConfig->serial_no;
    zlog_category_t *log =((HTTPDServer *)arg)->log;
    Stringex body = Stringex_null;
    Stringex auth = Stringex_null;
    ServerResponse *response;
    ServerResponseCreate(&response,log, request, arg);

    if (WechatHandlerShippingBody(&body, response, appid, mchid, notify_url))
    {

        if (WechatConfigAuthorizationBuild(&auth, "POST", "/v3/pay/transactions/jsapi", apiclient_key, mchid, serial_no, body))
        {

            HTTPSUtilsCallback callback;
            HTTPSUtilsRequestPost("https://api.mch.weixin.qq.com/v3/pay/transactions/jsapi", auth, body, &callback);
            WechatHandlerShippingResult(response, apiclient_key, appid, callback.memory);
            if (callback.memory)
                free(callback.memory);
        }
        else
        {
            ServerResponseByErrorMessage(response, 400, "构建Authorization失败");
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "构建Body失败");
    }

    ServerResponseDelete(response);
    StringexDelete(body);
    StringexDelete(auth);
}