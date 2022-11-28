#if !defined(DEVELOPER_KERNEL_H)
#define DEVELOPER_KERNEL_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <developer/public.h>
#include <developer/allocate.h>
#include <developer/conf.h>
#include <developer/args.h>


typedef struct
{
    char *name;
    allocate_t *allocate;
    args_t *args;
    conf_t *conf;
    char *serverURI;
    boolean error_log_printf;
    long maxInflight; 
} kernel_t;

ok_t initializing(kernel_t **kernel, size_t allocate_max_size, int argc, char *argv[]);
void cleanup(kernel_t *kernel);

#endif