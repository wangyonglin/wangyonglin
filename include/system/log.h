#if !defined(__SYSTEM_LOG_H__)
#define __SYSTEM_LOG_H__

#include <system/types.h>
#include <system/pool.h>

typedef int log_int_t;
#define DJ_LOG_OK ((log_int_t)0)
#define DJ_LOG_NULL ((log_int_t)-1)
#define DJ_LOG_ERROR ((log_int_t)-2)
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
    config_bool_t print_model;
} config_log_t;

ok_t system_log_initializing(allocate_pool_t *pool, const char *name, config_bool_t model, config_log_t **log);

int system_log_error(config_log_t *log, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
#endif //!__LOG__H__