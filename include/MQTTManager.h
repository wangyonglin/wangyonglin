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
typedef struct
{
    // MQTTConfig_t *config;
    MQTTAsync client;
    MQTTAsync_connectOptions connectOptions;
    char *serverURI;
    char *clientId;
    int persistence_type;
    void *persistence_context;
    int finished;
    bool connected;
} MQTTManager_t;

ok_t MQTTManager_initializing(MQTTManager_t **manager, MQTTAliyun_Config_t * MQTTAliyun_Config);
void MQTTManager_cleanup(MQTTManager_t *manager);

void MQTTManager_serverURI(MQTTManager_t *manager, const char *serverURI);
void MQTTManager_clientId(MQTTManager_t *manager, const char *clientId);

void MQTTManager_finished(MQTTManager_t *manager, int finished);
void MQTTManager_connected(MQTTManager_t *manager, bool connected);

void MQTTManager_automaticReconnect(MQTTManager_t *manager, int automaticReconnect);
void MQTTManager_onSuccess(MQTTManager_t *manager, MQTTAsync_onSuccess *onSuccess);
void MQTTManager_onFailure(MQTTManager_t *manager, MQTTAsync_onFailure *onFailure);
void MQTTManager_Passwrod(MQTTManager_t *manager, const char *password);
void MQTTManager_Username(MQTTManager_t *manager, const char *username);
void MQTTManager_maxInflight(MQTTManager_t *manager, int maxInflight);
void MQTTManager_context(MQTTManager_t *manager, void * context);
void MQTTManager_keepAliveInterval(MQTTManager_t *manager, int keepAliveInterval);
void MQTTManager_cleansession(MQTTManager_t *manager, int cleansession);

void MQTTManager_maxRetryInterval(MQTTManager_t *manager, int maxRetryInterval);
void MQTTManager_minRetryInterval(MQTTManager_t *manager, int minRetryInterval);
void MQTTManager_connectOptions_initializer(MQTTManager_t *manager);
int MQTTManager_application(MQTTManager_t *manager);
#endif