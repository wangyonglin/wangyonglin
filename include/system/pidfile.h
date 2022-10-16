#if !defined(__SYSTEM_PIDFILE_H__)
#define __SYSTEM_PIDFILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <system/log.h>
#include <system/types.h>
typedef bool config_lock_flag_t;
typedef struct
{
    char *name;
    config_fd_t fd;
    config_bool_t onlock;
} config_lock_t;

typedef struct
{
    char *name;
    config_fd_t fd;
    config_bool_t alone_runnind;
} config_pidfile_t;


config_pidfile_t *config_pidfile_allocate(config_pidfile_t **pidfile);
void config_pidfile_deallocate(config_pidfile_t *pidfile);
ok_t config_pidfile_initializing(config_pidfile_t *pidfile, const char *filename);
flag_t config_pidfile_listene(config_pidfile_t *pidfile);

ok_t config_pidfile_crt(config_pidfile_t *pidfile);
ok_t config_pidfile_del(config_pidfile_t *pidfile);
void config_pidfile_kill(config_pidfile_t *pidfile);
#endif //__TIGER_PID__H__