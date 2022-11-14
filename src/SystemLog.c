#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <SystemLog.h>
#include <SystemString.h>

#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

ok_t SystemLog_initializing(SystemLog_t **SystemLog,AllocateUtils_t *AllocateUtils, const char *name, bool model)
{
    if (!AllocateUtils && name)
    {
        return ArgumentException;
    }

    int len = strlen(name);
    if (((*SystemLog) = AllocateUtils_pool(AllocateUtils, sizeof(SystemLog_t))) == NULL)
    {
        return NullPointerException;
    }

    if ((AllocateUtils_toString(&((*SystemLog)->name), AllocateUtils, strdup(name), strlen(name))) != OK)
    {
        return NullPointerException;
    }
    (*SystemLog)->model = model;
    return OK;

    return ErrorException;
}

void SystemLog_clean(){


}
int SystemLog_error(SystemLog_t *SystemLog,const char *fmt, ...)
{
    if (!SystemLog)
    {
        return ArgumentException;
    }
    FILE *fptr;
    char log_line[1000] = {0};
    size_t log_line_pos;
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    log_line_pos = strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);

    if (SystemLog)
    {
        if (SystemLog->model == 0)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(SystemLog->name, "a+");
        }

        if (fptr)
        {

            if (SystemLog->model == 0)
            {
                fprintf(fptr, FONT_COLOR_RED);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [ERROR]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (SystemLog->model == 0)
            {
                fprintf(fptr, COLOR_NONE);
            }

            fputc('\r', fptr);
            fputc('\n', fptr);
            fflush(fptr);
            va_end(va);
        }
    }

    return log_line_pos;
}
int SystemLog_info(SystemLog_t *SystemLog,const char *fmt, ...)
{
    if (!SystemLog)
    {
        return ArgumentException;
    }
    FILE *fptr;
    char log_line[1000] = {0};
    size_t log_line_pos;
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    log_line_pos = strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);

    if (SystemLog)
    {
        if (SystemLog->model == 0)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(SystemLog->name, "a+");
        }

        if (fptr)
        {

            if (SystemLog->model == 0)
            {
                fprintf(fptr, FONT_COLOR_GREEN);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [INFO]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (SystemLog->model == 0)
            {
                fprintf(fptr, COLOR_NONE);
            }

            fputc('\r', fptr);
            fputc('\n', fptr);
            fflush(fptr);
            va_end(va);
        }
    }

    return log_line_pos;
}

/*
int log_info(log_t *log, const char *fmt, ...)
{
    FILE *fptr;
    char log_line[1000] = {0};
    size_t log_line_pos;
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    log_line_pos = strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
    if (log)
    {
        if (log->print_model == 0)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(log->name, "a+");
        }
        if (fptr)
        {
            if (log->print_model == 0)
            {
                fprintf(fptr, FONT_COLOR_GREEN);
            }
            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [INFO]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);
            if (log->print_model == 0)
            {
                fprintf(fptr, COLOR_NONE);
            }
            fputc('\r', fptr);
            fputc('\n', fptr);
            fflush(fptr);
            va_end(va);
        }
    }
    return log_line_pos;
}
int log_write(log_t *log, log_priority_t priority, const char *fmt, ...)
{
    FILE *fptr;
    char log_line[1000] = {0};
    size_t log_line_pos;
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    log_line_pos = strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
    if (log)
    {
        if (log->print_model == 0)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(log->name, "a+");
        }
        if (fptr)
        {
            switch (priority)
            {
            case LOG_ERR:
                if (log->print_model == 0)
                {
                    fprintf(fptr, FONT_COLOR_RED);
                }
                fprintf(fptr, log_line);
                fprintf(fptr, "%s\t", " [ERROR]");
                break;
            case LOG_INFO:
                if (log->print_model == 0)
                {
                    fprintf(fptr, FONT_COLOR_GREEN);
                }
                fprintf(fptr, log_line);
                fprintf(fptr, "%s\t", " [INFO]");
                break;
            case LOG_DEBUG:
                if (log->print_model == 0)
                {
                    fprintf(fptr, FONT_COLOR_BLUE);
                }
                fprintf(fptr, log_line);
                fprintf(fptr, "%s\t", " [DEBUF]");
                break;
            default:
                if (log->print_model == 0)
                {
                    fprintf(fptr, FONT_COLOR_WHITE);
                }
                fprintf(fptr, log_line);
                fprintf(fptr, "%s\t", " [NONE]");
                break;
            }
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);
            if (log->print_model == 0)
            {
                fprintf(fptr, COLOR_NONE);
            }
            fputc('\r', fptr);
            fputc('\n', fptr);
            fflush(fptr);
            va_end(va);
        }
    }
    return log_line_pos;
}
*/