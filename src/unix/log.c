#include <unix/log.h>

log_t *log = NULL;

log_t *log_crt(const char *filename)
{
    log_t *log = (log_t *)malloc(sizeof(log_t));
    if (log)
    {
        log->fptr = fopen(filename, "a+");
        return log;
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
        free(log);
    }
}

int log_write(unsigned int priority, const char *fmt, ...)
{

    char log_line[1000];
    size_t log_line_pos;
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = localtime(&rawtime);
    log_line_pos = strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);

    if (log->fptr)
    {

        fprintf(log->fptr, log_line);
        switch (priority)
        {
        case LOG_ERR:
            fprintf(log->fptr, "%s\t", " ERROR : ");
            break;
        case LOG_INFO:
            fprintf(log->fptr, "%s\t", " INFO :  ");
            break;
        case LOG_DEBUG:
            fprintf(log->fptr, "%s\t", " DEBUF : ");
            break;
        default:
            fprintf(log->fptr, "%s\t", " NONE :  ");
            break;
        }
        
        va_list va;
        int rc;
        va_start(va, fmt);
        vfprintf(log->fptr, fmt, va);
        fputc('\r', log->fptr);
        fputc('\n', log->fptr);
        fflush(log->fptr);
        va_end(va);
    }
    return log_line_pos;
}
