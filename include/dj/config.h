#if !defined(__DJ_CONFIG_H__)
#define __DJ_CONFIG_H__
#include <dj/types.h>
#include <dj/args.h>
#include <dj/log.h>
#include <dj/lock.h>
typedef struct
{
    dj_log_t *log;
    dj_args_t *args;
    dj_lock_t *lock;
} dj_config_t;

dj_config_t *dj_config_init(int argc, char *argv[]);

void dj_config_clean(dj_config_t *dconfig);
#endif