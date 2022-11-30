#if !defined(DEVELOPER_PROGRAM_H)
#define DEVELOPER_PROGRAM_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <developer/public.h>
#include <developer/allocate.h>
#include <developer/conf.h>
#include <developer/args.h>
#include <developer/pidlock.h>
#include <developer/daemon.h>
#include <developer/logger.h>

typedef struct
{
    char *name;
    allocate_t *allocate;
    args_t *args;
    conf_t *conf;
    logger_t *logger;
} program_t;

ok_t program_initializing(program_t **program, size_t allocate_max_size, int argc, char *argv[]);
void program_cleanup(program_t *program);

#endif