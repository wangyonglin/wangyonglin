#if !defined(__MQTT_CONFIG_H_)
#define __MQTT_CONFIG_H_
#include <wangyonglin/config.h>
#include <mqtt/conf.h>
typedef ok_t (*message_cb_t)(config_t *config, const char *addr, char *id, const char *usr, const char *pw, const char *topicName, void *payload, int payloadlen, int *rc);
typedef struct
{
    mqtt_conf_t *conf;
    config_t *config;
    message_cb_t message;
} mqtt_config_t;

mqtt_config_t *mqtt_config_init(config_t *config, mqtt_config_t **mconfig);

ok_t mqtt_config_message_cb(mqtt_config_t *mconfig, ok_t (*message_cb_t)(config_t *config, const char *addr, char *id, const char *usr, const char *pw, const char *topicName, void *payload, int payloadlen, int *rc));
void mqtt_config_free(mqtt_config_t *mconfig);
#endif