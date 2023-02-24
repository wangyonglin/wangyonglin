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
#include <wangyonglin/application.h>
#include <wangyonglin/list.h>
#include <cJSON.h>
#include <aliutils.h>
ok_t aliapisPub(char **aliapisurl,aliutils_sys *sys, char *MessageContentText, size_t MessageContentSize);

#endif