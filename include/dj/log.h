#if !defined(__DJ_LOG_H__)
#define __DJ_LOG_H__

#include <dj/types.h>
#define DJ_LOG_OK ((dj_int_t)0)
#define DJ_LOG_NULL ((dj_int_t)-1)
#define DJ_LOG_ERROR ((dj_int_t)-2)
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
    int print_model;
} dj_log_t;

dj_int_t dj_log_init(dj_log_t **log, const char *name, dj_int_t model);
void dj_log_clean(dj_log_t *log);
int dj_log_error(dj_log_t *log, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int dj_log_info(dj_log_t *log, const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int dj_log_write(dj_log_t *log, log_priority_t priority, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

#endif //!__LOG__H__