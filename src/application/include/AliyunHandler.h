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
#include <ArrayList.h>
#include <cJSON.h>
#include <AliyunUtils.h>
#include <AliyunConfig.h>

#include <Stringex.h>
Boolean HTTPSAliyunPub(cJSON **RetCallback, AliyunConfig *aliConfig, cJSON *DeviceName, cJSON *MessageContentText);
Boolean HTTPSAliyunRegisterDevice(cJSON **RetCallback, AliyunConfig *aliConfig, char *DeviceName);
Boolean HTTPSAliyunGetDeviceStatus(cJSON **RetCallback, AliyunConfig *aliConfig, char *DeviceName);
#endif