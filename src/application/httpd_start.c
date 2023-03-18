#include <application/httpd.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <wangyonglin/regedit.h>
#include <wangyonglin/object.h>
#include <wangyonglin/buffer.h>
#include <application/httpd_handler.h>

httpd_t *httpd = NULL;
httpd_t *httpd_config_create(datasheet cfg,log_t *log);
void httpd_config_delete();
localapis_t *aliapis;

httpd_t *httpd_config_create(datasheet cfg,log_t *log)
{

    if (object_create((void **)&httpd, sizeof(httpd_t)))
    {
        object_command_t objects[] = {
            object_string_command("address", offsetof(httpd_t, address)),
            object_integer_command("port", offsetof(httpd_t, port)),
            object_integer_command("timeout_in_secs", offsetof(httpd_t, timeout_in_secs)),
            object_null_command};
        object_mirror(objects, httpd, cfg, "HTTPD");
        localapis_init(&aliapis, cfg, "ALIIOT");
        httpd->aliapis = aliapis;
        httpd->log=log;
        return httpd;
    }

    return httpd;
}

void httpd_start(datasheet cfg,log_t *log)
{

    if (httpd_config_create(cfg,log))
    {
        event_init();
        httpd->https = evhttp_start(httpd->address, httpd->port);
        evhttp_set_timeout(httpd->https, httpd->timeout_in_secs);
        evhttp_set_cb(httpd->https, "/aliyun/publish", PublishHandler, httpd);
        evhttp_set_cb(httpd->https, "/aliyun/regisgerdevice", RegisterDeviceHandler, httpd);
        evhttp_set_gencb(httpd->https, error_handler, httpd);
        event_dispatch();
        httpd_config_delete();
    }
}
void httpd_config_delete()
{
    if (httpd->https)
    {
        buffer_delete(httpd->address);
        evhttp_free(httpd->https);
        object_delete(httpd);
    }
    localapis_free(aliapis);
}
