#if !defined(INCLUDE_WANGYONGLIN_OPTIONS_H)
#define INCLUDE_WANGYONGLIN_OPTIONS_H

#include <wangyonglin/package.h>
#include <wangyonglin/pool.h>
#include <wangyonglin/string.h>
#include <wangyonglin/wangyonglin.h>

typedef struct _options_t
{
    unsigned char *cfname;
    boolean startup;
    boolean daemonize;
    char *error_log;
    boolean log_errors;
    char *lockfile;
} options_t;

options_t *options_create(struct _pool_t *pool, int argc, char *argv[]);
#endif