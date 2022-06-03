#if !defined(__HTTPS_CONFIG_H__)
#define __HTTPS_CONFIG_H__
#include <wangyonglin/config.h>
#include <https/conf.h>

typedef struct
{
    https_conf_t *conf;
    struct evhttp *httpd;
    config_t *config;
} https_config_t;

https_config_t *https_config_init(config_t *config, https_config_t **hconfig);
void https_config_free(https_config_t *hconfig);
#endif