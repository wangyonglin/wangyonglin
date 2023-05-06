#include <Tiktok/TiktokConfig.h>
#include <WebServer.h>
#include <ServerResponse.h>

Boolean TiktokHandlerTastStartInit(
    TiktokCallbackResponse *tiktokCallbackResponse,
    ServerResponse *response,
    Stringex tiktokAppID,
    Stringex tiktokSecret,
    Stringex tiktokPublicKey)
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

    Stringex tiktokRoomID = Stringex_null;
    ServerResponseQuery(&tiktokRoomID, response, "roomid");
    if (!Stringex_IsString(tiktokRoomID) && !Stringex_IsEmpty(tiktokRoomID))
    {
        zlog_warn(log, "Stringex tiktokRoomID 参数不能为空\r\n");
        return err;
    }

    Stringex tiktokAccessToken = Stringex_null;
    ServerResponseQuery(&tiktokAccessToken, response, "access_token");
    if (!Stringex_IsString(tiktokAccessToken) && !Stringex_IsEmpty(tiktokAccessToken))
    {
        zlog_warn(log, "Stringex tiktokAccessToken 参数不能为空\r\n");
        return err;
    }
    Stringex tiktokBody = Stringex_null;
    cJSON *root = cJSON_CreateObject();
    if (root)
    {
        cJSON_AddStringToObject(root, "roomid", tiktokRoomID.valuestring);
        cJSON_AddStringToObject(root, "appid", tiktokAppID.valuestring);
        cJSON_AddStringToObject(root, "msg_type", "live_comment");
        char *jsonstring = cJSON_PrintUnformatted(root);
        if (jsonstring)
        {
            size_t jsonstringlength = strlen(jsonstring);
            StringexCreate(&tiktokBody, jsonstring, jsonstringlength);
        }
        cJSON_Delete(root);
    }
    Stringex tiktokAuthorization = Stringex_null;

    if (TiktokAuthorization(&tiktokAuthorization, "POST", "/api/live_data/task/start", tiktokAppID, tiktokPublicKey, tiktokBody))
    {
        if (Stringex_IsEmpty(tiktokAuthorization))
        {
            zlog_info(log, tiktokAuthorization.valuestring);
        }
        TiktokPostToken(
            "https://webcast.bytedance.com/api/live_data/task/start",
            tiktokAccessToken,
            tiktokAuthorization,
            tiktokBody,
            tiktokCallbackResponse);

        err = Boolean_true;
    }

    StringexDelete(tiktokBody);
    StringexDelete(tiktokAuthorization);
    StringexDelete(tiktokRoomID);
    StringexDelete(tiktokAccessToken);
    return err;
}

void TiktokHandlerTastStart(
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
    if (TiktokHandlerTastStartInit(&tiktokCallbackResponse, response, tiktokAppID, tiktokSecret, tiktokPublicKey))
    {

        if (tiktokCallbackResponse.memory)
        {
            cJSON *root = NULL;
            zlog_info(log, tiktokCallbackResponse.memory);
            root = cJSON_Parse(tiktokCallbackResponse.memory);
            ServerResponseBySuccessfly(response, root);
            StringDelete(tiktokCallbackResponse.memory);
        }
    }
    else
    {
        ServerResponseByErrorMessage(response, 400, "未知错误");
    }

    ServerResponseDelete(response);
}