#if !defined(__SYSTEM_ERROR_H__)
#define __SYSTEM_ERROR_H__
#include <SystemTypes.h>


void err_printf(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void error_info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
#endif