#include <wangyonglin/log.h>
#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

log_t *log_create(struct _pool_t *pool, struct _conf_t *cf)
{
    struct _log_t *log;
    size_t logsize = 0;
    if (!pool && !cf->error_log)
    {
        return log = NULL;
    }
    if (log = allocate(pool, sizeof(struct _log_t)))
    {
        if (cf->error_log[0] == '/')
        {
            string_create(pool, &log->error_log, cf->error_log, strlen(cf->error_log));
        }
        else
        {
            logsize += strlen(PACKAGE_DIRECTERY_PREFIX);
            logsize += strlen(cf->error_log);
            logsize += 2;
            log->error_log = allocate(pool, logsize);
            strcat(log->error_log, PACKAGE_DIRECTERY_PREFIX);
            strcat(log->error_log, "/");
            strcat(log->error_log, cf->error_log);
        }
        log->log_errors = cf->log_errors;
        return log;
    }

    return log = NULL;
}

void logerr(log_t *log, const char *fmt, ...)
{
    if (log && log->log_errors == positive && log->error_log)
    {
        char log_line[1000] = {0};
        time_t rawtime;
        struct tm *info;
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
        FILE *fptr;
        if ((fptr = fopen(log->error_log, "a+")))
        {
            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [ERROR]");
            va_list va;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);
            fputc('\r', fptr);
            fputc('\n', fptr);
            fflush(fptr);
            va_end(va);
            fclose(fptr);
        }
    }
}