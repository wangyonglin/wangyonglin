#include <developer/logger.h>

#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

conf_command_t logger_conf_commands[] = {
    {"logger_filename", String, offsetof(logger_t, logger_filename)}};

ok_t logger_initializing(logger_t **logger, allocate_t *allocate, conf_t *conf, boolean model)
{
    if (!allocate && !conf)
    {
        return ArgumentException;
    }

    if (object_create(allocate, (void**)logger, sizeof(logger_t) != Ok))
    {
        return ErrorException;
    }
    int logger_conf_commands_size = sizeof(logger_conf_commands) / sizeof(logger_conf_commands[0]);

    if (conf_mapping(conf, (void **)logger, sizeof(logger_t), NULL, logger_conf_commands, logger_conf_commands_size) == Ok)
    {
        (*logger)->allocate = allocate;
        (*logger)->logger_model = model;
        return Ok;
    }
    return ErrorException;
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
        if (logger->logger_model == disabled)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(logger->logger_filename, "a+");
        }

        if (fptr)
        {

            if (logger->logger_model == disabled)
            {
                fprintf(fptr, FONT_COLOR_RED);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [ERROR]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (logger->logger_model == disabled)
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
        if (logger->logger_model == disabled)
        {
            fptr = stderr;
        }
        else
        {
            fptr = fopen(logger->logger_filename, "a+");
        }

        if (fptr)
        {

            if (logger->logger_model == disabled)
            {
                fprintf(fptr, FONT_COLOR_GREEN);
            }

            fprintf(fptr, log_line);
            fprintf(fptr, "%s\t", " [INFO]");
            va_list va;
            int rc;
            va_start(va, fmt);
            vfprintf(fptr, fmt, va);

            if (logger->logger_model == disabled)
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
