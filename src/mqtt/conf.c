#include <wangyonglin/config.h>
#include <mqtt/conf.h>
#include <wangyonglin/log.h>

mqtt_conf_t * mqtt_conf_new(config_t *config, mqtt_conf_t **conf)
{
    (*conf) = (mqtt_conf_t *)global_hooks.allocate(sizeof(mqtt_conf_t));
    if (config->conf && (*conf))
    {
        if (config_get_string(config, "MQTT", "mqtt_address",&((*conf)->address)) == ok)
        {
            log_write(config, LOG_INFO, (*conf)->address);
        }
        if (config_get_number(config, "MQTT", "mqtt_port", &((*conf)->port)) == ok)
        {
            log_write(config, LOG_INFO, "{%d}", (*conf)->port);
        }
        if (config_get_number(config, "MQTT", "mqtt_keepaliveInterval",&((*conf)->keepaliveInterval)) == ok)
        {
            log_write(config, LOG_INFO, "{%d}", (*conf)->keepaliveInterval);
        }
        if (config_get_string(config, "MQTT", "mqtt_username",&((*conf)->username)) == ok)
        {
            log_write(config, LOG_INFO, (*conf)->username);
        }
        if (config_get_string(config, "MQTT", "mqtt_password", &((*conf)->password)) == ok)
        {
            log_write(config, LOG_INFO, (*conf)->password);
        }
        return  (*conf);
    }
    return NULL;
}

void mqtt_conf_free(mqtt_conf_t *conf)
{
    if (conf)
    {
        global_hooks.deallocate(conf);
    }
}