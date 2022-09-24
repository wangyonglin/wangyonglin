#if !defined(__UNIX_LOCK_H__)
#define __UNIX_LOCK_H__

#include <stdio.h>
#include <stdlib.h>
#include <dj/log.h>

typedef struct
{
    char *name;
    dj_fd_t fd;
} dj_lock_t;

#define DJ_LOCK_OK ((dj_int_t)0)
#define DJ_LOCK_EXIST ((dj_int_t)1)
#define DJ_LOCK_NULL ((dj_int_t)-1)
#define DJ_LOCK_ERROR ((dj_int_t)-2)


dj_int_t dj_lock_init(dj_lock_t **lock, dj_log_t *log, const char *name);
void dj_lock_clean(dj_lock_t *lock);
dj_int_t dj_lock_exist(dj_lock_t *lock, dj_log_t *log);
dj_int_t dj_lock_crt(dj_lock_t *lock, dj_log_t *log,int pid);
void dj_lock_del(dj_lock_t *lock);

dj_int_t dj_unlink(const char *name);
dj_int_t dj_kill(const char *name);
#endif //__TIGER_PID__H__