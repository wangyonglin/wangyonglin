#include <HTTPSServer.h>
#include <SystemString.h>
#include <SnowFlake.h>
#include <curl/curl.h>
#include <cJSON.h>
ConfUtils_command_t HTTPSServer_commands[] = {
    {"address", string, offsetof(HTTPSServer_t, address)},
    {"port", integer, offsetof(HTTPSServer_t, port)},
    {"timeout_in_secs", integer, offsetof(HTTPSServer_t, timeout_in_secs)}};
    
int HTTPSServer_commands_size = sizeof(HTTPSServer_commands) / sizeof(HTTPSServer_commands[0]);

ok_t HTTPSServer_initializing(HTTPSServer_t **HTTPSServer, SystemConfig_t *SystemConfig)
{
    if (!SystemConfig->AllocateUtils && !SystemConfig->ConfUtils)
    {
        return ArgumentException;
    }
    (*HTTPSServer) = AllocateUtils_pool(SystemConfig->AllocateUtils, sizeof(HTTPSServer_t));
    if (!(*HTTPSServer))
    {
        return NullPointerException;
    }

    ConfUtils_final(SystemConfig->ConfUtils, (void **)HTTPSServer, sizeof(HTTPSServer_t), "HTTPS", HTTPSServer_commands, HTTPSServer_commands_size);
    SystemLog_info(SystemConfig->SystemLog, "address    {%s}", (*HTTPSServer)->address);
    SystemLog_info(SystemConfig->SystemLog, "port    {%d}", (*HTTPSServer)->port);
    SystemLog_info(SystemConfig->SystemLog, "timeout_in_secs    {%d}", (*HTTPSServer)->timeout_in_secs);
    SnowFlake_initializing(100);
    // 初始化event API
    event_init();
    // 创建一个HTTPserver
    (*HTTPSServer)->httpd = evhttp_start((*HTTPSServer)->address, (*HTTPSServer)->port);
    // 设置超时时间15秒
    evhttp_set_timeout((*HTTPSServer)->httpd, (*HTTPSServer)->timeout_in_secs);
    (*HTTPSServer)->SystemConfig = SystemConfig;
    return OK;
}
ok_t HTTPSServer_router(HTTPSServer_t *HTTPSServer)
{
    if (HTTPSServer)
    {
        evhttp_set_cb(HTTPSServer->httpd, "/v1/login", login_handler, HTTPSServer);
        evhttp_set_cb(HTTPSServer->httpd, "/v3/pay/transactions/jsapi", v3_pay_transactions_jsapi, HTTPSServer);
        // 循环监听
        event_dispatch();
        return OK;
    }

    return ErrorException;
}
void HTTPSServer_cleanup(HTTPSServer_t *HTTPSServer)
{
    if (HTTPSServer)
    {
        evhttp_free(HTTPSServer->httpd);
    }
}
