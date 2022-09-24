#if !defined(__UNIX_TYPES_H__)
#define __UNIX_TYPES_H__
#include <stdio.h>
#include <stdlib.h>

typedef int dj_int_t;
typedef int dj_fd_t;
typedef char dj_char_t;

#define DJ_OK ((dj_int_t)0)
#define DJ_ERROR ((dj_int_t)-1)
#define DJ_AGAIN ((dj_int_t)-2)
#define DJ_BUSY ((dj_int_t)-3)
#define DJ_DONE ((dj_int_t)-4)
#define DJ_DECLINED ((dj_int_t)-5)
#define DJ_ABORT ((dj_int_t)-6)
#define DJ_INVALID_FILE ((dj_int_t)-7)
#define DJ_EACCES  ((dj_int_t)-8)

typedef int dj_bool_t;
#define DJ_BOOL_TRUE ((dj_bool_t)1)
#define DJ_BOOL_FALSE ((dj_bool_t)0)

typedef char * dj_string_t;
#endif //__UNIX_TYPES_H__