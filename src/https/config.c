#include <wangyonglin/config.h>
#include <https/config.h>

https_config_t *https_config_init(config_t *config, https_config_t **hconfig)
{
    if (config)
    {
        (*hconfig) = (https_config_t *)global_hooks.allocate(sizeof(https_config_t));
        if ((*hconfig))
        {
            memset(*hconfig, 0x00, sizeof(https_config_t));
            if (https_conf_new(config, &((*hconfig)->conf)))
            {
                (*hconfig)->config = config;
                return (*hconfig);
            }
            else
            {
                global_hooks.deallocate(*hconfig);
                return NULL;
            }
        }
    }
    return NULL;
}
void https_config_free(https_config_t *hconfig)
{
    if (hconfig)
    {
        if (hconfig->conf)
        {
            https_conf_free(hconfig->conf);
        }
        global_hooks.deallocate(hconfig);
    }
}