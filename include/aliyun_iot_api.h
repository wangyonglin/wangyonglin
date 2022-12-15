#if !defined(INCLUDE_ALIYUN_IOT_API_H)
#define INCLUDE_ALIYUN_IOT_API_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"
#include <developer/config.h>

#include <cJSON.h>

typedef struct
{
    char *Format;
    char *Version;
    char *AccessKeyId;
    char *Signature;
    char *SignatureMethod;
    char *Timestamp;
    char *SignatureVersion;
    char *SignatureNonce;
    char *RegionId;
    long maxInflight;
    boolean AliOK;
} AliPubParams_t;

ok_t AliPubParamsCreate(AliPubParams_t **AliPubParams,config_t *config);
ok_t AliPubParamsInit(AliPubParams_t *AliPubParams, config_t *config);
void AliPubParamsDelete(AliPubParams_t *AliPubParams);

ok_t AliPubParams_toString(config_t *config);
#endif