#if !defined(__SYSTEM_ERROR_H__)
#define __SYSTEM_ERROR_H__
#include <AllocateUtils.h>
ok_t SystemError_Message(const char *fmt, ...)  __attribute__((format(printf, 1, 2)));
ok_t SystemError_exitMessage(AllocateUtils_t *AllocateUtils, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
#endif