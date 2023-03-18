#include <wangyonglin/log.h>
#include <wangyonglin/object.h>
#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"
FILE *logopen(log_t *log, const char *filename);

log_t *log_create(log_t **log)
{

    if (object_create((void **)log, sizeof(log_t)))
    {
        (*log)->logdir = datasheet_create(PACKAGE_DIRECTERY_LOG, strlen(PACKAGE_DIRECTERY_LOG));
        if (datasheet_value((*log)->logdir))
        {
            mkdir((*log)->logdir.string, 0755);
            return (*log);
        }
        object_delete((*log));
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
        FILE *fptr = logopen(log, ".error.log");
        if (fptr)
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

void loginfo(log_t *log, const char *fmt, ...)
{
    if (log)
    {

        char log_line[20] = {0};
        time_t rawtime;
        struct tm *info;
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
        FILE *fptr = logopen(log, ".access.log");
        if (fptr)
        {
            fprintf(fptr, log_line);
            fprintf(fptr, "\t");
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

FILE *logopen(log_t *log, const char *filename)
{
    size_t logmax = 0;
    FILE *fptr = NULL;
    if (log)
    {
        if (datasheet_value(log->logdir))
        {
            logmax += datasheet_length(log->logdir);
        }

        time_t rawtime;
        struct tm *info;
        time(&rawtime);
        info = localtime(&rawtime);
        char log_line[32] = {0};
        logmax += strftime(log_line, sizeof(log_line), "%Y-%m-%d", info);
        strcat(log_line, filename);
        logmax++;
        logmax++;
        char tmpString[logmax];
        memset(&tmpString, 0x00, sizeof(tmpString));
        strcat(tmpString, datasheet_value(log->logdir));
        strcat(tmpString, "/");
        strcat(tmpString, log_line);
        if ((fptr = fopen(tmpString, "a+")))
        {
            return fptr;
        }
    }
    return NULL;
}

void log_delete(log_t *log)
{
    if (log)
    {
        datasheet_delete(log->logdir);
        object_delete(log);
    }
}