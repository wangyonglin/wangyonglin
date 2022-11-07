#include <HTTPSServer.h>
#include <SystemString.h>
#include <SnowFlake.h>
#include <curl/curl.h>
#include <cJSON.h>





httpd_config_t *httpd_initializing(httpd_config_t **httpd, const char *address, uint16_t port)
{
    if (allocate((void **)httpd, sizeof(httpd_config_t)))
    {
        SnowFlake_initializing(100);
        //初始化event API
        event_init();
        //创建一个HTTPserver
        (*httpd)->httpd = evhttp_start(address, port);
        //设置超时时间15秒
        evhttp_set_timeout((*httpd)->httpd, 15);

        return (*httpd);
    }
    return NULL;
}
ok_t httpd_router(httpd_config_t *httpd, void *this)
{
    if (httpd)
    {
        evhttp_set_cb(httpd->httpd, "/v1/login", login_handler, this);
        evhttp_set_cb(httpd->httpd, "/v3/pay/transactions/jsapi", v3_pay_transactions_jsapi, this);
        //循环监听
        event_dispatch();
        return OK;
    }

    return ErrorException;
}
void httpd_cleanup(httpd_config_t *httpd)
{
    if (httpd)
    {
        evhttp_free(httpd->httpd);
        deallocate(httpd);
    }
}
