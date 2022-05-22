#ifndef __LOG__H__
#define __LOG__H__
#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
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

ok_t log_init(config_t *config);
int log_write(config_t *config, unsigned int priority, const char *fmt, ...) __attribute__((format(printf, 3, 4)));

#endif //!__LOG__H__