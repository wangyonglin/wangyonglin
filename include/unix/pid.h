#if !defined(__UNIX_PID_H__)
#define __UNIX_PID_H__

#include <stdio.h>
#include <stdlib.h>
#include <unix/log.h>
typedef struct
{
    char *filename;
} pid_mk_t;

pid_mk_t *pid_crt(pid_mk_t **pid, log_t *log, const char *filename);
void pid_del(pid_mk_t *pid, log_t *log);
#endif //__TIGER_PID__H__