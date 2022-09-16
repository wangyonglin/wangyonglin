#include <stdarg.h>
#include <time.h>
#include <unix/log.h>
#include <unix/string.h>

#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

log_t *log_crt(log_t **log, const char *filename, int print_model)
{
    if ((*log) = (log_t *)global_hooks.allocate(sizeof(log_t)))
    {
        (*log)->print_model = print_model;
        if ((*log)->print_model == 0)
        {
            (*log)->fptr = stderr;
            return (*log);
        }
        else
        {
            if ((*log)->fptr = fopen(filename, "a+"))
                return (*log);
        }

        global_hooks.deallocate((*log));
    }
    return NULL;
}

void log_del(log_t *log)
{
    if (log)
    {
        if (log->fptr)
        {
            fclose(log->fptr);
        }
        global_hooks.deallocate(log);
    }
}

int log_write(log_t *log, log_priority_t priority, const char *fmt, ...)
{
    if (log)
    {

        time_t rawtime;
        struct tm *info;
        time(&rawtime);
        info = localtime(&rawtime);
        log->log_line_pos = strftime(log->log_line, sizeof(log->log_line), "%Y-%m-%d %H:%M:%S", info);

        if (log->fptr)
        {

            switch (priority)
            {
            case LOG_ERR:
                if (log->print_model == 0)
                {
                    fprintf(log->fptr, FONT_COLOR_RED);
                }

                fprintf(log->fptr, log->log_line);
                fprintf(log->fptr, "%s\t", " [ERROR]");
                break;
            case LOG_INFO:
                if (log->print_model == 0)
                {
                    fprintf(log->fptr, FONT_COLOR_GREEN);
                }
                fprintf(log->fptr, log->log_line);
                fprintf(log->fptr, "%s\t", " [INFO]");
                break;
            case LOG_DEBUG:
                if (log->print_model == 0)
                {
                    fprintf(log->fptr, FONT_COLOR_BLUE);
                }
                fprintf(log->fptr, log->log_line);
                fprintf(log->fptr, "%s\t", " [DEBUF]");
                break;
            default:
                if (log->print_model == 0)
                {
                    fprintf(log->fptr, FONT_COLOR_WHITE);
                }
                fprintf(log->fptr, log->log_line);
                fprintf(log->fptr, "%s\t", " [NONE]");
                break;
            }

            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(log->fptr, fmt, va);
            if (log->print_model == 0)
            {
                fprintf(log->fptr, COLOR_NONE);
            }

            fputc('\r', log->fptr);
            fputc('\n', log->fptr);
            fflush(log->fptr);
            va_end(va);
        }
    }

    return log->log_line_pos;
}
