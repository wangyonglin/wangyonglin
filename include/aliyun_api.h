#if !defined(INCLUDE_ALIYUN_API_H)
#define INCLUDE_ALIYUN_API_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"
#include <developer/config.h>
#include <wangyonglin/listitem.h>
#include <cJSON.h>

typedef struct _aliyun_config
{
    char *DeviceName;
    char *AccessKeySecret;
    char *Action;
    char *ProductKey;
    char *TopicFullName;
    char *Format;
    char *Version;
    char *AccessKeyId;
    char *Signature;
    char *SignatureMethod;
    char *SignatureVersion;
    char *RegionId;
    long maxInflight;
} aliyun_config;

ok_t aliyun_config_init(aliyun_config **config,const char * filename);
ok_t aliyun_config_publish(aliyun_config *config, char *MessageContentText, size_t MessageContentSize);

#endif