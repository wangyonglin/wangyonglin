#include <Tiktok/TiktokSignature.h>
#include <StringexSha256WithRsa.h>

#include <ServerResponse.h>

Boolean TiktokHandlerAccessTokenInit(
    TiktokCallbackResponse *tiktokCallbackResponse,
    ServerResponse *response,
    Stringex tiktokAppID, Stringex tiktokSecret, Stringex tiktokPublicKey)
{
    Boolean err = Boolean_false;
    zlog_category_t *log = response->log;

    if (!response)
    {
        zlog_warn(log, "ServerResponse *response 参数不能为空");
        return err;
    }
    if (!Stringex_IsString(tiktokAppID) && !Stringex_IsEmpty(tiktokAppID))
    {
        zlog_warn(log, "Stringex tiktokAppID 参数不能为空\r\n");
        return err;
    }
    if (!Stringex_IsString(tiktokSecret) && !Stringex_IsEmpty(tiktokSecret))
    {
        zlog_warn(log, "Stringex tiktokSecret 参数不能为空\r\n");
        return err;
    }
    if (!Stringex_IsString(tiktokPublicKey) && !Stringex_IsEmpty(tiktokPublicKey))
    {
        zlog_warn(log, "Stringex tiktokPublicKey 参数不能为空\r\n");
        return err;
    }

    Stringex tiktokBody = Stringex_null;
    cJSON *root = cJSON_CreateObject();
    if (root)
    {
        cJSON_AddStringToObject(root, "appid", tiktokAppID.valuestring);
        cJSON_AddStringToObject(root, "secret", tiktokSecret.valuestring);
        cJSON_AddStringToObject(root, "grant_type", "client_credential");
        char *jsonstring = cJSON_PrintUnformatted(root);
        if (jsonstring)
        {
            size_t jsonstringlength = strlen(jsonstring);
            StringexCreate(&tiktokBody, jsonstring, jsonstringlength);
        }
        cJSON_Delete(root);
    }
    Stringex tiktokAuthorization = Stringex_null;

    if (TiktokAuthorization(&tiktokAuthorization, "POST", "/api/apps/v2/token", tiktokAppID, tiktokPublicKey, tiktokBody))
    {
        if (Stringex_IsEmpty(tiktokAuthorization))
        {
            zlog_info(log, tiktokAuthorization.valuestring);
        }
        TiktokPost(
            "https://developer.toutiao.com/api/apps/v2/token",
            tiktokAuthorization,
            tiktokBody,
            tiktokCallbackResponse);

        err = Boolean_true;
    }

    StringexDelete(tiktokBody);
    StringexDelete(tiktokAuthorization);
    return err;
}

void TiktokHandlerAccessToken(
    struct evhttp_request *request,
    void *arg)
{
    Boolean err = Boolean_false;
    Stringex tiktokAppID = ((WebServer *)arg)->tiktokConfig->appid;
    Stringex tiktokSecret = ((WebServer *)arg)->tiktokConfig->secret;
    Stringex tiktokPublicKey = ((WebServer *)arg)->tiktokConfig->public_key;

    zlog_category_t *log = ((WebServer *)arg)->log;

    ServerResponse *response;
    ServerResponseCreate(&response, log, request, arg);
    TiktokCallbackResponse tiktokCallbackResponse;
    if (TiktokHandlerAccessTokenInit(&tiktokCallbackResponse, response, tiktokAppID, tiktokSecret, tiktokPublicKey))
    {
        if (tiktokCallbackResponse.memory)
        {
            zlog_info(log, tiktokCallbackResponse.memory);
            cJSON *root = cJSON_Parse(tiktokCallbackResponse.memory);
            ServerResponseBySuccessfly(response, root);
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "未知错误");
    }

    ServerResponseDelete(response);
}
