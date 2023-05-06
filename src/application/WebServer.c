#include <WebServer.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <HTTPDHandler.h>
#include <WechatHandler.h>
#include <WechatHandlerNotifyUrl.h>
#include <WechatHandlerShipping.h>
#include <zlog/zlog.h>
#include <Tiktok/TiktokConfig.h>
#include <Tiktok/Tiktok.h>
#include <ServerResponse.h>

WebServer *webServer = NULL;

void error_handler(struct evhttp_request *request, void *arg)
{
    ServerResponse *response;
    ServerResponseCreate(&response, NULL, request, arg);
    ServerResponseByErrorMessage(response, 404, "not paga 404");
    ServerResponseDelete(response);
}
WebServer *WebServerCreate(MainConfig *mainConfig)
{
    if (mainConfig && mainConfig->injectObject)
    {
        if (ObjectCreate((void **)&webServer, sizeof(WebServer)))
        {
            InjectCommand injectCommand[] = {
                INJECT_COMMAND_STRING("address", offsetof(WebServer, address)),
                INJECT_COMMAND_INTEGER("port", offsetof(WebServer, port)),
                INJECT_COMMAND_INTEGER("timeout_in_secs", offsetof(WebServer, timeout_in_secs)),
                INJECT_COMMAND_NULL};
            InjectCommandInit(mainConfig->injectObject, injectCommand, webServer, "HTTPD");
            webServer->log = mainConfig->log;
            return webServer;
        }
    }

    return webServer;
}

void HTTPDServerStart(MainConfig *mainConfig)
{

    if (WebServerCreate(mainConfig))
    {
        zlog_category_t *log = mainConfig->log;
        AliyunConfigCreate(mainConfig, &webServer->aliConfig, "ALIIOT");
        WechatConfigCreate(mainConfig, &webServer->wConfig, "WECHAT_PAYMENT");
        TiktokConfigCreate(&webServer->tiktokConfig, mainConfig, "TIKTOK");

        Stringex mchid = webServer->wConfig->mchid;
        Stringex serial_no = webServer->wConfig->serial_no;
        Stringex apiclient_key = webServer->wConfig->apiclient_key;
        Stringex apiv3_key = webServer->wConfig->apiv3_key;

        if (!WechatConfigCertificatesCreate(&webServer->configCertificates, mchid, serial_no, apiclient_key, apiv3_key))
        {
            zlog_error(log, "获取微信平台证书失败");
            return;
        }
        event_init();

        if (!(webServer->https = evhttp_start(webServer->address.valuestring, webServer->port)))
        {
            zlog_error(log, "HTTPD服务启动失败");
            return;
        }
        else
        {
            zlog_info(log, "HTTPD服务启动成功    IP地址:%s 端口:%d", webServer->address.valuestring, webServer->port);
        }
        evhttp_set_timeout(webServer->https, webServer->timeout_in_secs);
        evhttp_set_cb(webServer->https, "/jscode2session/openid", WechatHandlerOpenId, webServer);
        evhttp_set_cb(webServer->https, "/v3/pay/transactions/jsapi", WechatHandlerShipping, webServer);
        evhttp_set_cb(webServer->https, "/wechat/notify_url", WechatHandlerNotifyUrl, webServer);
        evhttp_set_cb(webServer->https, "/aliyun/user/set", HTTPDAliyunUserSet, webServer);
       // evhttp_set_cb(webServer->https, "/aliyun/device/regisger", HTTPDAliyunRegisterDevice, webServer);
        //evhttp_set_cb(webServer->https, "/aliyun/device/status", HTTPDAliyunGetDeviceStatus, webServer);
        evhttp_set_cb(webServer->https, "/tiktok/access/token", TiktokHandlerAccessToken, webServer);
        evhttp_set_cb(webServer->https, "/tiktok/tast/start", TiktokHandlerTastStart, webServer);
        evhttp_set_gencb(webServer->https, error_handler, webServer);
        event_dispatch();
        WechatConfigCertificatesDelete(webServer->configCertificates);
        WechatConfigDelete(webServer->wConfig);
        AliyunConfigDelete(webServer->aliConfig);
        TiktokConfigDelete(webServer->tiktokConfig);
        WebServerDelete();
    }
}
void WebServerDelete()
{
    if (webServer->https)
    {

        StringexDelete(webServer->address);
        evhttp_free(webServer->https);
        ObjectDelete(webServer);
    }
}
