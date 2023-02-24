#include <wangyonglin/log.h>
#define COLOR_NONE "\033[0m"
#define FONT_COLOR_WHITE "\033[0;37m"
#define FONT_COLOR_RED "\033[0;31m"
#define FONT_COLOR_GREEN "\033[0;32m"
#define FONT_COLOR_BLUE "\033[1;34m"

log_t *log_create(struct _app_t *app)
{
  if(!object_create(app->pool,&app->log,sizeof(struct _log_t))){
        return NULL;
    }
    if(!app->options){
         return NULL;
    }
    if (!string_create(app->pool,&app->log->error_log,app->options->error_log,strlen(app->options->error_log)))
    {
        return NULL;
    }
    app->log->log_errors=app->options->log_errors;

    return app->log;
}

void logerr(log_t *log, const char *fmt, ...)
{
    if (log && log->log_errors == positive && log->error_log)
    {
        char log_line[1000] = {0};
        time_t rawtime;
        struct tm *info;
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(log_line, sizeof(log_line), "%Y-%m-%d %H:%M:%S", info);
        FILE *fptr;
        if ((fptr = fopen(log->error_log, "a+")))
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