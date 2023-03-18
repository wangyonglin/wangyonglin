#if !defined(WANGYONGLIN_WANGYONGLIN_H)
#define WANGYONGLIN_WANGYONGLIN_H

#include <wangyonglin/package.h>
#include <wangyonglin/pool.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/options.h>
#include <wangyonglin/lock.h>
#include <wangyonglin/daemonize.h>
#include <wangyonglin/log.h>
#include <wangyonglin/message.h>
#include <wangyonglin/string.h>
#include <wangyonglin/conf.h>
typedef struct _config_t
{
    options_t *options;
    log_t *log;
} config_t;

config_t *config_create(config_t **config, int argc, char *argv[]);


#endif