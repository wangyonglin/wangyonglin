#include <HTTPDServer.h>

#include <curl/curl.h>
#include <cJSON.h>
#include <HTTPDHandler.h>

#include <wangyonglin/wangyonglin.h>

HTTPDServer *server = NULL;
AliyunConfig *aliConfig;

HTTPDServer *HTTPDServerCreate(config_t *config)
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
            server->log = config->log;
            return server;
        }
    }

    return server;
}

void HTTPDServerStart(config_t *config)
{

    if (HTTPDServerCreate(config))
    {
        printf("[%s=>%d]\r\n", server->address.valuestring, server->port);

        WechatPaymentCreate(config, &server->payment, "WECHAT_PAYMENT");
        event_init();
        server->https = evhttp_start(server->address.valuestring, server->port);
        evhttp_set_timeout(server->https, server->timeout_in_secs);
        evhttp_set_cb(server->https, "/wechat/jscode2session", HTTPDrWechatJscode2session, server);
        evhttp_set_cb(server->https, "/wechat/purchase", HTTPDrWechatPurchase, server);
        evhttp_set_cb(server->https, "/wechat/notify_url", HTTPDrWechatNotifyUrl, server);
        // evhttp_set_cb(httpd->https, "/aliyun/publish", PublishHandler, httpd);
        evhttp_set_cb(server->https, "/aliyun/regisgerdevice", HTTPDAliyunRegisterDevice, server);
        evhttp_set_gencb(server->https, error_handler, server);
        event_dispatch();
        WechatPaymentDelete(server->payment);
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
