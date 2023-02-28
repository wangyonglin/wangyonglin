#if !defined(INCLUDE_DEVELOPER_LOG_H)
#define INCLUDE_DEVELOPER_LOG_H
#include <wangyonglin/package.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/string.h>
#include <wangyonglin/wangyonglin.h>
typedef struct _log_t{
    char *error_log;
    boolean log_errors;
}log_t;

log_t *log_create(struct _pool_t *pool, struct _conf_t *cf);
void logerr(log_t *log, const char *fmt, ...);
#endif