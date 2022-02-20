#ifndef __LOG__H__
#define __LOG__H__
#include <wangyonglin/linux.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/config.h>

/* ============================================================
 * Logging functions
 * ============================================================ */
int log_init(config_t *config);
void log_free(config_t *config);
int log_echo(config_t *config, unsigned int priority, const char *fmt, ...) __attribute__((format(printf, 3, 4)));
int log_vprintf(config_t *config, unsigned int priority, const char *fmt, va_list va);
#endif //!__LOG__H__