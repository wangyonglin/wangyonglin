#include <HTTPDServer.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <HTTPDHandler.h>
#include <wangyonglin/wangyonglin.h>
#include <WechatHandler.h>
#include <HTTPDResult.h>
#include <WechatHandlerNotifyUrl.h>
#include <WechatHandlerShipping.h>
#include <zlog/zlog.h>
HTTPDServer *server = NULL;
AliyunConfig *aliConfig;
void error_handler(struct evhttp_request *request, void *arg)
{
    HTTPDServer *httpd = (HTTPDServer *)arg;
    ResultUtil *ResUtil = NULL;
    ResultUtilCreate(&ResUtil);
    httpd_result_t *result;
    httpd_result_create(&result, request, arg);
    ResultUtilFailure(ResUtil, 404, "404 not found");
    httpd_result_complete(result, ResUtil);
    ResultUtilDelete(ResUtil);
    httpd_result_delete(result);
}
HTTPDServer *HTTPDServerCreate(Config_t *config)
{
    if (config && config->inject)
    {
        if (object_create((void **)&server, sizeof(HTTPDServer)))
        {
            inject_command_t commands[] = {
                inject_string_command("address", offsetof(HTTPDServer, address)),
                inject_integer_command("port", offsetof(HTTPDServer, port)),
                inject_integer_command("timeout_in_secs", offsetof(HTTPDServer, timeout_in_secs)),
                inject_null_command};
            inject_build(config->inject, commands, server, "HTTPD");
            server->log = config->zlog;
            return server;
        }
    }

    return server;
}

void HTTPDServerStart(Config_t *config)
{

    if (HTTPDServerCreate(config))
    {
        AliyunConfigCreate(config, &server->aliConfig, "ALIIOT");
        WechatConfigCreate(config, &server->wConfig, "WECHAT_PAYMENT");
        zlog_category_t *log = config->zlog;
        Stringex mchid = server->wConfig->mchid;
        Stringex serial_no = server->wConfig->serial_no;
        Stringex apiclient_key = server->wConfig->apiclient_key;
        Stringex apiv3_key = server->wConfig->apiv3_key;

        if (!WechatConfigCertificatesCreate(&server->configCertificates, mchid, serial_no, apiclient_key, apiv3_key))
        {
            zlog_error(log, "获取微信平台证书失败");
            return;
        }
        event_init();

        if (!(server->https = evhttp_start(server->address.valuestring, server->port)))
        {
            zlog_error(log, "HTTPD服务启动失败");
            return;
        }
        else
        {
            zlog_info(log, "HTTPD服务启动成功    IP地址:%s 端口:%d", server->address.valuestring, server->port);
        }
        evhttp_set_timeout(server->https, server->timeout_in_secs);
        evhttp_set_cb(server->https, "/jscode2session/openid", WechatHandlerOpenId, server);
        evhttp_set_cb(server->https, "/v3/pay/transactions/jsapi", WechatHandlerShipping, server);
        evhttp_set_cb(server->https, "/wechat/notify_url", WechatHandlerNotifyUrl, server);
        evhttp_set_cb(server->https, "/aliyun/user/set", HTTPDAliyunUserSet, server);
        evhttp_set_cb(server->https, "/aliyun/device/regisger", HTTPDAliyunRegisterDevice, server);
        evhttp_set_cb(server->https, "/aliyun/device/status", HTTPDAliyunGetDeviceStatus, server);
        evhttp_set_gencb(server->https, error_handler, server);
        event_dispatch();
        WechatConfigCertificatesDelete(server->configCertificates);
        WechatConfigDelete(server->wConfig);
        AliyunConfigDelete(server->aliConfig);
        HTTPDServerDelete();
    }
}
void HTTPDServerDelete()
{
    if (server->https)
    {

        string_delete(server->address);
        evhttp_free(server->https);
        object_delete(server);
    }
}
