#if !defined(INCLUDE_WANGYONGLIN_OPTIONS_H)
#define INCLUDE_WANGYONGLIN_OPTIONS_H

#include <wangyonglin/package.h>
#include <wangyonglin/pool.h>
#include <wangyonglin/string.h>
typedef struct _options_t
{

    string_t confname;
    boolean startup;
    boolean daemonize;
    string_t error_log;
    boolean log_errors;
    string_t lockfile;
} options_t;

options_t *options_create(options_t **options, pool_t *pool, int argc, char *argv[]);
#endif