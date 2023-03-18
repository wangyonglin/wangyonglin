#if !defined(INCLUDE_ALIAPIS_H)
#define INCLUDE_ALIAPIS_H
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
#include <application/aliutils.h>
#include<application/localapis.h>

string_t aliapis_https_publish(localapis_t *aliapis, char *DeviceName, char *MessageContentText, size_t MessageContentSize);
string_t aliapis_https_register_device(localapis_t *aliapis, char *DeviceName);

ok_t QueryDeviceInfo(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName);
ok_t QueryDeviceStatistics(char **aliapisurl, struct _aliutils_apis_t *apis);
ok_t GetDeviceStatus(char **aliapisurl, struct _aliutils_apis_t *apis, char *IotId);
#endif