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
ok_t aliapis_https_pub(char **aliapisurl, struct _aliutils_apis_t *apis, char *MessageContentText, size_t MessageContentSize);

#endif