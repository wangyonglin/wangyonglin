#if !defined(DEVELOPER_PIDLOCK_H)
#define DEVELOPER_PIDLOCK_H
#include <signal.h>
#include <developer/public.h>
#include <developer/allocate.h>
#include <developer/conf.h>
typedef struct
{
    char *filename;
    boolean statused;
    allocate_t *allocate;
    conf_t *conf;
    int lockfd;
} pidlock_t;

ok_t pidlock_initializing(pidlock_t **pidlock, allocate_t *allocate,conf_t *conf);
ok_t pidlock_status(pidlock_t *pidlock);
ok_t pidlock_locking(pidlock_t *pidlock);
ok_t pidlock_unlocking(pidlock_t *pidlock);
ok_t pidlock_exit(pidlock_t *pidlock);
#endif