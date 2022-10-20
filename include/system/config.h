#if !defined(__SYSTEM_CONFIG_H__)
#define __SYSTEM_CONFIG_H__
#include <system/types.h>

#include <system/log.h>
#include <system/pidfile.h>
#include <system/conf.h>
#include <system/options.h>
#include <system/pool.h>

typedef struct
{
    config_log_t *log;
    config_conf_t *conf;
    system_options_t *options;
    config_pidfile_t *pidfile;
    callback_quit_t *callback_quit;
    allocate_pool_t *pool;
} config_t;

config_t *config_allocate(config_t **config);
void config_deallocate(config_t *config);
// ok_t config_initializing(config_t *config, int argc, char *argv[]);
ok_t config_management(config_t *config);
ok_t config_destory(config_t *config);

ok_t system_config_initializing(config_t **config, int argc, char *argv[]);
void system_config_cleanup(config_t *config);
#endif