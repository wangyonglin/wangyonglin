#if !defined(INCLUDE_MQTTALIYUNCONFIG_H)
#define INCLUDE_MQTTALIYUNCONFIG_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <SystemConfig.h>
#include <SystemTypes.h>
#include <MQTTDriver.h>
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


ok_t MQTTAliyun_Signature(const char *productKey, const char *deviceName, const char *deviceSecret, char clientID[150], char username[64], char password[65]);
#endif