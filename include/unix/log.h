#if !defined(__UNIX_LOG_H__)
#define __UNIX_LOG_H__

#include <stdio.h>
#include <stdlib.h>

typedef enum
{
    LOG_ERR = -1,
    LOG_INFO,
    LOG_NONE,
    LOG_DEBUG,
} log_priority_t;

typedef struct
{
    FILE *fptr;
    char *filename;
    int print_model;
    char log_line[1000];
    size_t log_line_pos;
} log_t;

log_t *log_crt(log_t **log, const char *filename, int print_model);
void log_del(log_t *log);
int log_write(log_t *log, log_priority_t priority, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

#endif //!__LOG__H__