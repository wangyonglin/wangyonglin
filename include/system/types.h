#if !defined(__UNIX_TYPES_H__)
#define __UNIX_TYPES_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
typedef int __int_t;
typedef int __fd_t;
typedef char __char_t;

#define TYPES_OK ((__int_t)0)
#define TYPES_ERROR ((__int_t)-1)
#define TYPES_AGAIN ((__int_t)-2)
#define TYPES_BUSY ((__int_t)-3)
#define TYPES_DONE ((__int_t)-4)
#define TYPES_DECLINED ((__int_t)-5)
#define TYPES_ABORT ((__int_t)-6)
#define TYPES_INVALID_FILE ((__int_t)-7)
#define TYPES_EACCES ((__int_t)-8)

typedef int config_bool_t;
#define TRUE ((config_bool_t)1)
#define FALSE ((config_bool_t)0)

typedef enum
{
    on_start,
    on_stop
} on_starting_t;

typedef int flag_t;
#define FLAG_TRUE ((ok_t)1)
#define FLAG_FALSE ((ok_t)0)
#define FLAG_ERROR ((ok_t)-1)

typedef int ok_t;
#define ok ((ok_t)1)
#define ok_err_failure ((ok_t)-1)
#define ok_err_none ((ok_t)0)
#define ok_err_exit ((ok_t)-2)
#define ok_err_onlock ((ok_t)-3)
#define ok_quit ((ok_t)-4)

typedef int config_fd_t;
typedef struct
{
    int (*resolve)(int level, void *args);
    void (*reject)();
} config_callback_t;

typedef void(*callback_quit_t)(void * pvParameters);
#endif //__UNIX_TYPES_H__