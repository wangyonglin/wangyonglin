#if !defined(INCLUDE_DEVELOPER_LOG_H)
#define INCLUDE_DEVELOPER_LOG_H
#include <wangyonglin/package.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/string.h>

typedef struct _log_t
{
    char* error_log;
    boolean log_errors;
} log_t;

log_t *log_create(log_t **log, pool_t *pool, conf_t *conf);
void logerr(log_t *log, const char *fmt, ...);
#endif