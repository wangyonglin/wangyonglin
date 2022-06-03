#if !defined(__MQTT_CONF_H__)
#define __MQTT_CONF_H__
#include <wangyonglin/config.h>
typedef   ok_t (*mqtt_send_t)(config_t *config,const char * addr, const char *usr,const char*pw, const char *topicName, void *payload, int payloadlen, int *rc);
typedef struct
{
    char *address;
    long port;
    long keepaliveInterval;
    char *username;
    char *password;
    long timeout;
    void *args;

} mqtt_conf_t;

mqtt_conf_t *mqtt_conf_new(config_t *config, mqtt_conf_t **conf);

void mqtt_conf_free(mqtt_conf_t *conf);
#endif /**__MQTT_CONF_H__**/