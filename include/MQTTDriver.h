#if !defined(INCLUDE_MQTTDRIVER_H)
#define INCLUDE_MQTTDRIVER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"
#include <MQTTAliyunConfig.h>
#include <SystemString.h>
typedef struct
{
    char *serverURI;
    char *clientID;
    char *productKey;
    char *deviceName;
    char *deviceSecret;
    long minRetryInterval;
    long maxRetryInterval;
    long cleansession;
    long keepAliveInterval;
    long automaticReconnect;
    long maxInflight;
} MQTTDriver_t;

typedef MQTTAsync_connectOptions MQTTDriver_connectOptions_t;
ok_t MQTTDriver_initializing(MQTTDriver_t **MQTTDriver, SystemAllocate_t *SystemAllocate, SystemConf_t *SystemConf, SystemLog_t *SystemLog);
void MQTTDriver_clean();
#endif