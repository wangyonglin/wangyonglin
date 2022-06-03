#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
#include <wangyonglin/log.h>
int log_vprintf(const char *filename, levels_t tag, unsigned int priority, const char *fmt, va_list va);

ok_t log_init(config_t *config)
{
  

    if (config && config->conf->handler)
    {
        config_get_string(config, NULL, "log_filename", &config->log_filename);
        config_get_levels(config, NULL, "log_filewrite", &config->log_tags);

        if (config->log_filename && (config->log_tags != None))
        {

            return ok;
        }
    }

    return failed;
}
int log_write(config_t *config, unsigned int priority, const char *fmt, ...)
{
    va_list va;
    int rc;
    va_start(va, fmt);
    rc = log_vprintf(config->log_filename, config->log_tags, priority, fmt, va);
    va_end(va);
    return rc;
}

int log_vprintf(const char *filename, levels_t tag, unsigned int priority, const char *fmt, va_list va)
{
    char log_line[1000];
    size_t log_line_pos;
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    FILE *fptr;
    info = localtime(&rawtime);
    log_line_pos = strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
    fptr = fopen(filename, "a+");
    if (fptr)
    {

        fprintf(fptr, log_line);
        switch (priority)
        {
        case LOG_ERR:
            fprintf(fptr, "%s\t", " ERROR : ");
            break;
        case LOG_INFO:
            fprintf(fptr, "%s\t", " INFO :  ");
            break;
        case LOG_DEBUG:
            fprintf(fptr, "%s\t", " DEBUF : ");
            break;
        default:
            fprintf(fptr, "%s\t", " NONE :  ");
            break;
        }

        if (tag == On)
        {
            vfprintf(fptr, fmt, va);
            fputc('\r', fptr);
            fputc('\n', fptr);
            fflush(fptr);
            fclose(fptr);
        }
        else
        {
            fprintf(stderr, fmt, va);
        }
    }
    return log_line_pos;
}
