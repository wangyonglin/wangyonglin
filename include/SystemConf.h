#if !defined(INCLUDE_SYSTEMCONF_H)
#define INCLUDE_SYSTEMCONF_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <SystemLog.h>

typedef struct
{
    CONF *conf;
    long errline;
    char *conffile;
} SystemConf_t;

ok_t SystemConf_initializing(SystemConf_t **SystemConf,allocate_pool_t *SystemAllocate, const char *filename);
void SystemConf_cleanup(SystemConf_t *SystemConf);
#endif