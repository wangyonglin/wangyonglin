#if !defined(DEVELOPER_PIDLOCK_H)
#define DEVELOPER_PIDLOCK_H
#include <signal.h>
#include <developer/public.h>
#include <developer/allocate.h>

#include <developer/mapping.h>

typedef struct
{
    char *pidfile;
    boolean statused;
    int lockfd;
    mapping_t *mapping;
} pidlock_t;

ok_t pidlock_initializing(pidlock_t **pidlock, mapping_t *mapping, char *cnf);
ok_t pidlock_status(pidlock_t *pidlock);
ok_t pidlock_locking(pidlock_t *pidlock);
ok_t pidlock_unlocking(pidlock_t *pidlock);
ok_t pidlock_exit(pidlock_t *pidlock);
#endif