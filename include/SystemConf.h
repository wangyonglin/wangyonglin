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
#include <SystemAllocate.h>
typedef struct
{
    CONF *conf;
    long errline;
    SystemAllocate_t *SystemAllocate;
} SystemConf_t;

ok_t SystemConf_initializing(SystemConf_t **SystemConf, SystemAllocate_t *SystemAllocate, const char *filename);
void SystemConf_clean(SystemConf_t *SystemConf);
ok_t SystemConf_String(SystemConf_t *SystemConf,char **pointer, const char *group, const char *name);
ok_t SystemConf_Number(SystemConf_t *SystemConf, long *pointer, const char *group, const char *name);
#endif