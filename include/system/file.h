#if !defined(__SYSTEM_FILE_H__)
#define __SYSTEM_FILE_H__
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <inttypes.h>
#include <system/log.h>
#include <system/types.h>

#define DJ_FILE_OK ((dj_int_t)0)
#define DJ_FILE_ERROR ((dj_int_t)-1)
#define DJ_FILE_EACCES ((dj_int_t)-2)
#define DJ_FILE_EAGAIN ((dj_int_t)-3)

#endif