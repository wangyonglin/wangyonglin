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
#include <ResultUtil.h>
boolean_by_t HTTPSAliyunPub(cJSON **RetCallback, AliyunConfig *aliConfig, cJSON *DeviceName, cJSON *MessageContentText);
boolean_by_t HTTPSAliyunRegisterDevice(cJSON **RetCallback, AliyunConfig *aliConfig, char *DeviceName);
boolean_by_t HTTPSAliyunGetDeviceStatus(cJSON **RetCallback, AliyunConfig *aliConfig, char *DeviceName);
#endif