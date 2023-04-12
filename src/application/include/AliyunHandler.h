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
string_by_t HTTPSAliyunPub(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName, char *MessageContentText, size_t MessageContentSize);
string_by_t HTTPSAliyunRegisterDevice(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName);
string_by_t HTTPSAliyunGetDeviceStatus(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName);
#endif