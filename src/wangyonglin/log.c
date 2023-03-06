#include <wangyonglin/log.h>
#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

log_t *log_create(log_t **log, pool_t *pool, conf_t *conf)
{
    if (pool && conf)
    {

        if (pool_object_create(pool, (void **)log, sizeof(struct _log_t)))
        {
            if (conf->error_log[0] == '/')
            {
                pool_buffer_create(pool, &(*log)->error_log, conf->error_log, strlen(conf->error_log));
            }
            else
            {
                size_t logsize = 0;
                logsize += strlen(PACKAGE_DIRECTERY_PREFIX);
                logsize += strlen(conf->error_log);
                logsize += 2;
                char tmpError_log[logsize];
                memset(tmpError_log, 0x00, sizeof(tmpError_log));
                strcat(tmpError_log, PACKAGE_DIRECTERY_PREFIX);
                strcat(tmpError_log, "/");
                strcat(tmpError_log, conf->error_log);
                pool_buffer_create(pool, &(*log)->error_log, tmpError_log, strlen(tmpError_log));
            }
            (*log)->log_errors = conf->log_errors;
            return (*log);
        }
    }

    return (*log) = NULL;
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