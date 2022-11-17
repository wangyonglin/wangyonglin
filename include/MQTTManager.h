#if !defined(INCLUDE_MQTTMANAGER_H)
#define INCLUDE_MQTTMANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"
#include <SystemTypes.h>
#include <MQTTAliyunSignature.h>
#include <SystemString.h>
#include <ConfUtils.h>
#include <SystemLog.h>
typedef struct
{
    // MQTTConfig_t *config;
    
    MQTTAsync client;
    int persistence_type;
    void *persistence_context;
    int finished;
    bool connected;
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
    SystemConfig_t *SystemConfig;
    bool daemon;
} MQTTManager_t;

ok_t MQTTManager_initializing(MQTTManager_t **MQTTManager, SystemConfig_t *SystemConfig);
void MQTTManager_cleanup(MQTTManager_t *MQTTManager,AllocateUtils_t *AllocateUtils);

int MQTTManager_application(MQTTManager_t *MQTTManager);
#endif