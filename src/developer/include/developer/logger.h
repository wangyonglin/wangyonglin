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

typedef struct
{
    allocate_t *allocate;
    char *filename;
    boolean model;
} logger_t;

int logerr(logger_t *logger, const char *fmt, ...);
int loginfo(logger_t *logger, const char *fmt, ...);
#endif