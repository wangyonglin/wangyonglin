#if !defined(DEVELOPER_PIDFILE_H)
#define DEVELOPER_PIDFILE_H
#include <signal.h>
#include <developer/public.h>
#include <developer/allocate.h>
typedef struct
{
    char *filename;
    boolean statused;
    allocate_t *allocate;
    int lockfd;
} pidfile_t;

ok_t pidfile_initializing(pidfile_t **pidfile, allocate_t *allocate, const char *filename);
ok_t pidfile_status(pidfile_t *pidfile);
ok_t pidfile_locking(pidfile_t *pidfile);
ok_t pidfile_unlocking(pidfile_t *pidfile);
ok_t pidfile_exit(pidfile_t *pidfile);
#endif