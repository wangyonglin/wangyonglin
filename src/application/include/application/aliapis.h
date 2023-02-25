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
ok_t Pub(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName, char *MessageContentText, size_t MessageContentSize);
ok_t QueryDeviceInfo(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName);
ok_t RegisterDevice(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName);
ok_t QueryDeviceStatistics(char **aliapisurl, struct _aliutils_apis_t *apis);
ok_t GetDeviceStatus(char **aliapisurl, struct _aliutils_apis_t *apis, char *IotId);
#endif