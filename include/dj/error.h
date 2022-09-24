#if !defined(__DJ_ERROR_H__)
#define __DJ_ERROR_H__
#include <dj/types.h>

#define DJ_ERROR_OK ((dj_int_t)0)
#define DJ_ERROR_NULL ((dj_int_t)-1)
#define DJ_ERROR_ERROR ((dj_int_t)-2)
void dj_debug(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
void dj_info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
#endif