#include <developer/log.h>
#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

ok_t log_create(log_t **log, allocate_t *allocate, char *filename, boolean model)
{

    if (!allocate)
    {
        perror("log_create allocate ArgumentException");
        return ArgumentException;
    }

    if (allocate_object_create(allocate, (void **)log, sizeof(log_t) != Ok))
    {
        perror("log_create log NullPointerException");
        return NullPointerException;
    }

    conf_command commands[] = {
        {"logfile", NULL, STRING, offsetof(log_t, logfile)}};
    int commands_size = conf_command_size(commands);

    if (conf_create((*log), filename, NULL, commands, commands_size) == Ok)
    {
        (*log)->model = model;
        return Ok;
    }
    return ErrorException;
}

int logerr(log_t *log, const char *fmt, ...)
{

    if (!log)
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

    if (log)
    {
        if (log->model == DISABLED)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(log->logfile, "a+");
        }

        if (fptr)
        {

            if (log->model == DISABLED)
            {
                fprintf(fptr, FONT_COLOR_RED);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [ERROR]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (log->model == DISABLED)
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