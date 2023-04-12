#if !defined(INCLUDE_DEVELOPER_LOG_H)
#define INCLUDE_DEVELOPER_LOG_H
#include <wangyonglin/package.h>
#include <string_by_this.h>


typedef struct _log_t
{
    string_by_t err;
    boolean_by_t log_errors;
} log_t;

log_t *log_create(log_t **log);
void logerr(log_t *log, const char *fmt, ...);
void loginfo(log_t *log, const char *fmt, ...);
void log_delete(log_t *log);

#endif