#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
#include <wangyonglin/log.h>
int main(int argc, char *argv[])
{
    const char *cfname = "/home/wangyonglin/github/wangyonglin/conf/wangyonglin.conf";

    config_t *config = config_new(cfname);
    if (config)
    {
        if (log_loader_config(config) == ok)
        {
            log_write(config, LOG_ERR, "erwefnwjefwefw");
        }

        config_free(config);
    }

    return 0;
}