#ifndef __HTTPS_CONF_H__
#define __HTTPS_CONF_H__
#include <wangyonglin/config.h>

typedef struct
{
    char *address;
    long port;
    long timeout_in_secs;
    char *pem_filename;
} https_conf_t;

https_conf_t * https_conf_new(config_t *config, https_conf_t **conf);
void https_conf_free(https_conf_t *conf);
 void login_handler(struct evhttp_request *req, void *arg);
#endif //!__HTTPS_CONF_H__