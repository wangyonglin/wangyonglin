#if !defined(INCLUDE_SYSTEMLOG_H)
#define INCLUDE_SYSTEMLOG_H

#include <SystemTypes.h>
#include <SystemPool.h>


typedef enum
{
    LOG_ERR = -1,
    LOG_INFO,
    LOG_NONE,
    LOG_DEBUG,
} log_priority_t;

typedef struct
{
    char *name;
    bool print_model;
} SystemLog_t;

ok_t SystemLog_initializing(SystemLog_t **SystemLog,allocate_pool_t *pool, const char *name, bool model);

int SystemLog_error(SystemLog_t *log, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
#endif //!__LOG__H__