#include <wangyonglin/log.h>

#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"
FILE *logopen(log_t *log, const char *filename);

log_t *log_create(log_t **log)
{
    if (((*log) = (log_t *)global_hooks.allocate(sizeof(log_t))))
    {
        mkdir(PACKAGE_DIRECTERY_LOG, 0755);
        size_t logmax = sizeof(PACKAGE_DIRECTERY_LOG) + sizeof(PACKAGE_TARNAME) + 8;
        char tmpString[logmax];
        memset(&tmpString, 0x00, sizeof(tmpString));
        strcat(tmpString, PACKAGE_DIRECTERY_LOG);
        strcat(tmpString, "/");
        strcat(tmpString, PACKAGE_TARNAME);
        strcat(tmpString, ".log");
        if (string_create(&(*log)->err, tmpString, strlen(tmpString)))
        {
            return (*log);
        }
        global_hooks.deallocate((*log));
    }
    return NULL;
}

void logerr(log_t *log, const char *fmt, ...)
{
    if (log)
    {

        char log_line[20] = {0};
        time_t rawtime;
        struct tm *info;
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
        FILE *fptr;
        if ((fptr = fopen(log->err.valuestring, "a+")))
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

void log_delete(log_t *log)
{
    if (log)
    {
        string_delete(log->err);
        object_delete(log);
    }
}