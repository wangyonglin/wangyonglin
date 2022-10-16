#if !defined(__SYSTEM_LOG_H__)
#define __SYSTEM_LOG_H__

#include <system/types.h>
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

config_log_t * config_log_allocate(config_log_t **log);
void config_log_deallocate(config_log_t*log);
ok_t config_log_initializing(config_log_t *log, const char *name, config_bool_t model);

ok_t config_log_init(config_log_t **log, const char *name, config_bool_t model);
void config_log_clean(config_log_t *log);
int config_log_error(config_log_t *log, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#endif //!__LOG__H__