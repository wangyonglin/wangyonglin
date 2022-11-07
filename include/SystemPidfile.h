#if !defined(INCLUDE_SYSTEMPIDFILE_H)
#define INCLUDE_SYSTEMPIDFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <SystemLog.h>
#include <SystemTypes.h>
#include <SystemPool.h>
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
    int fd;
    config_bool_t alone_runnind;
} SystemPidfile_t;

ok_t SystemPidfile_initializing(SystemPidfile_t **SystemPidfile, allocate_pool_t *pool, const char *filename);

ok_t SystemPidfile_listene(SystemPidfile_t *SystemPidfile);
ok_t SystemPidfile_crt(SystemPidfile_t *SystemPidfile);
ok_t SystemPidfile_del(SystemPidfile_t *SystemPidfile);
ok_t SystemPidfile_kill(SystemPidfile_t *SystemPidfile);


#endif //__TIGER_PID__H__