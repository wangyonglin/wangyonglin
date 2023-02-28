#if !defined(INCLUDE_WANGYONGLIN_CONF_H)
#define INCLUDE_WANGYONGLIN_CONF_H

#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#include <wangyonglin/wangyonglin.h>
// log_errors  = On
// error_log = "/home/wangyonglin/github/wangyonglin/logs/error.log"
// lockfile = "/var/run/wangyonglin.pid"
typedef struct _conf_t
{
    boolean log_errors;
    char *error_log;
    char *lockfile;
} conf_t;

conf_t *conf_create(struct _pool_t *pool, char *filename);

#endif