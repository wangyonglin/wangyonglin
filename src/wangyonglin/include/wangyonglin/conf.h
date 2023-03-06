#if !defined(INCLUDE_WANGYONGLIN_CONF_H)
#define INCLUDE_WANGYONGLIN_CONF_H

#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#include <wangyonglin/pool.h>
typedef struct _conf_t
{
    boolean log_errors;
    char *error_log;
    char *lockfile;
} conf_t;

conf_t *conf_create(conf_t **conf, pool_t *pool, string_t confname);

#endif