#if !defined(INCLUDE_APPLICATION_HTTPS_SUCCESSFUL_H)
#define INCLUDE_APPLICATION_HTTPS_SUCCESSFUL_H
#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#include <wangyonglin/object.h>
#include <wangyonglin/buffer.h>
#include <cJSON.h>

string_t https_failure(const char *errmsg);
string_t https_successful(cJSON *item);
#endif