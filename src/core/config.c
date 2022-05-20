#include <wangyonglin/config.h>
#include <wangyonglin/log.h>

config_t *config_new(const char *filename)
{
    config_t *config = (config_t *)global_hooks.allocate(sizeof(config_t));
    if (config)
    {
        memset(config, 0x00, sizeof(config_t));
        config->conf = conf_new(filename);
        if (config->conf)
        {
            return config;
        }
        else
        {
            global_hooks.deallocate(config);
        }
    }
    return NULL;
}


void config_free(config_t *config)
{
    if (config)
    {
        if (config->conf)
        {
            conf_free(config->conf);
        }
        global_hooks.deallocate(config);
    }
}
