#if !defined(__SYSTEM_CONF_H__)
#define __SYSTEM_CONF_H__
#include <toml/toml.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <system/log.h>

typedef struct
{
    CONF *conf;
    long errline;
    char *conffile;
} config_conf_t;

config_conf_t *config_conf_allocate(config_conf_t **conf);
void config_conf_deallocate(config_conf_t *conf);
ok_t config_conf_initializing(config_conf_t *conf, const char *filename);
#endif