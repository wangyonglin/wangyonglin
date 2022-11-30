#if !defined(DEVELOPER_LOGGER_H)
#define DEVELOPER_LOGGER_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <developer/public.h>
#include <developer/allocate.h>
#include <developer/conf.h>
typedef struct
{
    allocate_t *allocate;
    char *logger_filename;
    boolean logger_model;
} logger_t;

ok_t logger_initializing(logger_t **logger, allocate_t *allocate, conf_t *conf, boolean model);
int logerr(logger_t *logger, const char *fmt, ...);
int loginfo(logger_t *logger, const char *fmt, ...);
#endif