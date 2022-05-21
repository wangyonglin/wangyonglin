#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
#include <wangyonglin/daemon.h>
#include <wangyonglin/log.h>
int main(int argc, char *argv[])
{
    const char *cfname = "/home/wangyonglin/github/wangyonglin/conf/wangyonglin.conf";

    config_t *config = config_new(cfname);
    if (config)
    {
        if (log_loader_config(config) == ok)
        {
            if (daemond(config) == enabled)
            {
                log_write(config, LOG_INFO, "daemond success");
            }
            else
            {
                log_write(config, LOG_ERR, "daemond failure");
            }
        }

        config_free(config);
    }

    return 0;
}