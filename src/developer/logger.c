#include <developer/logger.h>

#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

ok_t logger_initializing(logger_t **logger, allocate_t *allocate, const char *filename, boolean model)
{
    if (!allocate && !filename)
    {
        return ArgumentException;
    }
    if (object_crt(allocate, logger, sizeof(logger_t) != Ok))
    {
        return ErrorException;
    }

    if ((string_crt(allocate, &(*logger)->filename, strdup(filename), strlen(filename))) != Ok)
    {
        return ErrorException;
    }
    (*logger)->allocate = allocate;
    (*logger)->model = model;
    return Ok;

}


int logerr(logger_t *logger, const char *fmt, ...)
{
    if (!logger)
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

    if (logger)
    {
        if (logger->model == disabled)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(logger->filename, "a+");
        }

        if (fptr)
        {

            if (logger->model == disabled)
            {
                fprintf(fptr, FONT_COLOR_RED);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [ERROR]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (logger->model == disabled)
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
int loginfo(logger_t *logger, const char *fmt, ...)
{
    if (!logger)
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

    if (logger)
    {
        if (logger->model == disabled)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(logger->filename, "a+");
        }

        if (fptr)
        {

            if (logger->model == disabled)
            {
                fprintf(fptr, FONT_COLOR_GREEN);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [INFO]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (logger->model == disabled)
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
