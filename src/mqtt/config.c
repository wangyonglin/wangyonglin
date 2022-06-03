#include <wangyonglin/config.h>
#include <mqtt/config.h>

mqtt_config_t *mqtt_config_init(config_t *config, mqtt_config_t **mconfig)
{
    (*mconfig) = (mqtt_config_t *)global_hooks.allocate(sizeof(mqtt_config_t));
    if (config && (*mconfig))
    {
        memset(*mconfig, 0x00, sizeof(mqtt_config_t));
        (*mconfig)->config = config;
        if (mqtt_conf_new(config, &((*mconfig)->conf)))
        {
            return *mconfig;
        }
        else
        {
            global_hooks.deallocate(*mconfig);
            return NULL;
        }
    }

    return NULL;
}
ok_t mqtt_config_message_cb(mqtt_config_t *mconfig, ok_t (*message_cb_t)(config_t *config, const char *addr, char *id, const char *usr, const char *pw, const char *topicName, void *payload, int payloadlen, int *rc)){
    if(mconfig){
        mconfig->message=message_cb_t;
        return ok;
    }
    return none;
}
void mqtt_config_free(mqtt_config_t *mconfig)
{
    if (mconfig)
    {
        if (mconfig->conf)
        {
            global_hooks.deallocate(mconfig->conf);
        }
        global_hooks.deallocate(mconfig);
    }
}