#if !defined(INCLUDE_DEVELOPER_LOG_H)
#define INCLUDE_DEVELOPER_LOG_H
#include <unistd.h>
#include <stdio.h>
#include <getopt.h> //getopt_long
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <developer/allocate.h>
#include <wangyonglin/conf.h>

typedef struct{
    char *logfile;
    boolean model;
}log_t;

ok_t log_create(log_t **log, allocate_t *allocate, char *filename, boolean model);
int logerr(log_t *log, const char *fmt, ...);
#endif