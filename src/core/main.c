#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
#include <wangyonglin/daemon.h>
#include <wangyonglin/log.h>
#include <wangyonglin/pid.h>
int main(int argc, char *argv[])
{
    const char *cfname = "/home/wangyonglin/github/wangyonglin/conf/wangyonglin.conf";

    config_t *config = config_new(cfname);
    if (config)
    {
        if (log_init(config) == ok)
        {
            if (daemond(config) == ok)
            {
                log_write(config, LOG_INFO, "daemon ok!");
            }
            else
            {
                log_write(config, LOG_INFO, "daemon failed!");
            }

            if (pid_crt(config) == ok)
            {
                log_write(config, LOG_INFO, "pid create ok!");
                if (application(config, NULL) == ok)
                {
                    log_write(config, LOG_INFO, "application ok!");
                }
                if (pid_del(config) == ok)
                {
                    log_write(config, LOG_INFO, "pid delect ok!");
                    config->pid_activated == disabled;
                }
            }
        }
        config_free(config);
    }

    return 0;
}