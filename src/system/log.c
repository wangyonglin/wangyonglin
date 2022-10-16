#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <system/log.h>
#include <system/string.h>

#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"
config_log_t *config_log_allocate(config_log_t **log)
{

    if (allocate_object((void **)log, sizeof(config_log_t)))
    {
        allocate_string(&((*log)->name), 512);
    }
}
void config_log_deallocate(config_log_t *log)
{
    deallocate_string(log->name);
    deallocate_object(log);
}
ok_t config_log_initializing(config_log_t *log, const char *name, config_bool_t model)
{
    if (log)
    {
        if (setting_string(log->name, name, strlen(name)))
        {
            log->print_model=model;
            return OK_SUCCESS;
        }
    }
    return OK_NONE;
}


int config_log_error(config_log_t *log, const char *fmt, ...)
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
                fprintf(fptr, FONT_COLOR_RED);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [ERROR]");
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