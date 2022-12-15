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
#include <developer/conf.h>

#include <developer/mapping.h>

typedef struct{
    char *logfile;
    boolean model;
    mapping_t *mapping;
}log_t;

ok_t log_initializing(log_t **log,mapping_t *mapping,char * filename, boolean model);
#endif