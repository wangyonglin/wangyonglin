#if !defined(INCLUDE_MQTTALIYUNCONFIG_H)
#define INCLUDE_MQTTALIYUNCONFIG_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <SystemConfig.h>
#include <SystemTypes.h>
#define SERVERURI_MAXLEN (150)
#define PRODUCTKEY_MAXLEN (20)
#define DEVICENAME_MAXLEN (32)
#define DEVICESECRET_MAXLEN (64)

#define SIGN_SOURCE_MAXLEN (200)
#define CLIENTID_MAXLEN (150)
#define USERNAME_MAXLEN (64)
#define PASSWORD_MAXLEN (65)

#define TIMESTAMP_VALUE "2524608000000"
#define MQTT_CLINETID_KV "|timestamp=2524608000000,_v=paho-c-1.0.0,securemode=3,signmethod=hmacsha256,lan=C|"

typedef struct
{
    char *serverURI;
    char *productKey;
    char *deviceName;
    char *deviceSecret;
    char clientID[CLIENTID_MAXLEN];
    char username[USERNAME_MAXLEN];
    char password[PASSWORD_MAXLEN];
    long minRetryInterval;
    long maxRetryInterval;
    long cleansession;
    long keepAliveInterval;
    long automaticReconnect;
    long maxInflight;
    SystemConfig_t *SystemConfig;
} MQTTAliyun_Config_t;
ok_t MQTTAliyun_Config_initializing(MQTTAliyun_Config_t **MQTTAliyun_Config, SystemConfig_t *SystemConfig);
ok_t MQTTAliyun_Config_Cleanup(MQTTAliyun_Config_t *MQTTAliyun_Config);
ok_t MQTTAliyun_Config_Signature(MQTTAliyun_Config_t *MQTTAliyun_Config);
#endif