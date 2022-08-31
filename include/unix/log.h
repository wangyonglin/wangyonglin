#ifndef __LOG__H__
#define __LOG__H__
#include <unix/unix.h>

/* Log types */
#define LOG_NONE 0
#define LOG_INFO (1 << 0)
#define LOG_NOTICE (1 << 1)
#define LOG_WARNING (1 << 2)
#define LOG_ERR (1 << 3)
#define LOG_DEBUG (1 << 4)
#define LOG_SUBSCRIBE (1 << 5)
#define LOG_UNSUBSCRIBE (1 << 6)
#define LOG_WEBSOCKETS (1 << 7)
#define LOG_INTERNAL 0x80000000U
#define LOG_ALL 0xFFFFFFFFU
typedef struct
{
    FILE *fptr;
} log_t;
log_t *log_crt(const char *filename);
void log_del(log_t *log);

int log_write(unsigned int priority, const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#endif //!__LOG__H__