#include <wangyonglin/config.h>
#include <wangyonglin/log.h>
#include <https/conf.h>
const char *group = "HTTPS";
https_conf_t * https_conf_new(config_t *config, https_conf_t **conf)
{   
    if (config->conf)
    {
        size_t len = sizeof(https_conf_t);
        if ((*conf) = (https_conf_t *)global_hooks.allocate(len))
        {
            memset((*conf), 0x00, len);
            if (config_get_string(config, group, "http_address", &((*conf)->address)) == ok)
            {
                log_write(config, LOG_INFO, (*conf)->address);
            }
            if (config_get_number(config, group, "http_port", &((*conf)->port)) == ok)
            {
                log_write(config, LOG_INFO, "{%d}", (*conf)->port);
            }
            if (config_get_number(config, group, "http_timeout_in_secs", &((*conf)->timeout_in_secs)) == ok)
            {
                log_write(config, LOG_INFO, "{%d}", (*conf)->timeout_in_secs);
            }
            return (*conf);
        }
    }
    return NULL;
}

void https_conf_free(https_conf_t *conf)
{
    if (conf)
    {
        if (conf->address)
            global_hooks.deallocate(conf->address);
        global_hooks.deallocate(conf);
    }
}