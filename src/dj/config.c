#include <dj/config.h>
#include <dj/string.h>
#include <dj/lock.h>
#include <string.h>
dj_config_t *dj_config_init(int argc, char *argv[])
{
    const char *pidfile = "/var/run/wangyonglin.pid";
    dj_config_t *config;
    if ((config = (dj_config_t *)global_hooks.allocate(sizeof(dj_config_t))))
    {
        memset(config, 0x00, sizeof(config));
        if (dj_args_init(&(config->args), argc, argv) == DJ_ARGS_OK)
        {
            if (config->args->started == DJ_STARTED_STOP)
            {
                if (dj_kill(pidfile) == DJ_LOCK_OK)
                {
                    dj_unlink(pidfile);
                    dj_info("kill ok");
                }
                dj_config_clean(config);
                exit(EXIT_SUCCESS);
            }
            printf("\tdeamon[%d]\r\n",config->args->deamon);
            if (config->args->deamon == DJ_BOOL_TRUE)
            {
                dj_daemon();
            }
            if (dj_log_init(&(config->log), "/home/wangyonglin/github/wangyonglin/logs/error.log", config->args->deamon) == DJ_LOG_OK)
            {
                dj_lock_init(&(config->lock), config->log, pidfile);
                if (dj_lock_exist(config->lock, config->log) == DJ_LOCK_EXIST)
                {
                    dj_config_clean(config);
                    exit(EXIT_SUCCESS);
                }
                return config;
            }
        }
    }
    return NULL;
}

void dj_config_clean(dj_config_t *dconfig)
{
    if (dconfig)
    {
        dj_lock_clean(dconfig->lock);
        dj_args_clean(dconfig->args);
        dj_log_clean(dconfig->log);
        global_hooks.deallocate(dconfig);
    }
}
