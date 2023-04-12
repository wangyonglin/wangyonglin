#if !defined(WANGYONGLIN_WANGYONGLIN_H)
#define WANGYONGLIN_WANGYONGLIN_H

#include <wangyonglin/package.h>
#include <wangyonglin/pool.h>

#include <wangyonglin/options.h>
#include <wangyonglin/lock.h>
#include <wangyonglin/daemonize.h>
#include <wangyonglin/log.h>
#include <wangyonglin/message.h>
#include <string_by_inject.h>

typedef struct _Config_t
{
    options_t *options;
    log_t *log;
    inject_t *inject;
    lock_t *lock;
} Config_t;

Config_t *ConfigCreate();
void ConfigDelete();
Config_t *ConfigInit(int argc, char *argv[]);

#endif