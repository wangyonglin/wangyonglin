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
#include <MQTTAliyunConfig.h>
#include <SystemString.h>
#include <MQTTDriver.h>
typedef struct
{
    // MQTTConfig_t *config;

    MQTTAsync client;
    int persistence_type;
    void *persistence_context;
    int finished;
    bool connected;
    SystemLog_t * SystemLog;
    MQTTDriver_t *MQTTDriver;
} MQTTManager_t;

ok_t MQTTManager_initializing(MQTTManager_t **MQTTManager, SystemAllocate_t *SystemAllocate, SystemConf_t *SystemConf, SystemLog_t *SystemLog);
void MQTTManager_cleanup(MQTTManager_t *MQTTManager);

int MQTTManager_application(MQTTManager_t *MQTTManager);
#endif