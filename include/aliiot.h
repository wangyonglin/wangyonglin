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
#include <developer/program.h>


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

} aliiot_t;

ok_t aliiot_initializing(aliiot_t **aliiot, program_t *program);
void aliiot_cleanup(aliiot_t *aliiot, program_t *program);
#endif