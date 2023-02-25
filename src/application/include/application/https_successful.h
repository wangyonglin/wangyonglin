#if !defined(INCLUDE_APPLICATION_HTTPS_SUCCESSFUL_H)
#define INCLUDE_APPLICATION_HTTPS_SUCCESSFUL_H
#include <wangyonglin/package.h>
#include <cJSON.h>
char *https_failure(char **ostr, const char *errmsg);
char *https_successful(char **pstr, cJSON *item);
#endif