#if !defined(INCLUDE_ALIYUNHANDLER_H)
#define INCLUDE_ALIYUNHANDLER_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include "MQTTAsync.h"
#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/list.h>
#include <cJSON.h>
#include <AliyunUtils.h>
#include <AliyunConfig.h>
#include <string_by_this.h>
// string_t aliapis_https_publish(localapis_t *aliapis, char *DeviceName, char *MessageContentText, size_t MessageContentSize);
string_by_t aliapis_https_register_device(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName);

// ok_t QueryDeviceInfo(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName);
// ok_t QueryDeviceStatistics(char **aliapisurl, struct _aliutils_apis_t *apis);
// ok_t GetDeviceStatus(char **aliapisurl, struct _aliutils_apis_t *apis, char *IotId);
#endif