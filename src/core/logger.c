#include <wangyonglin/config.h>
#include <wangyonglin/core.h>

struct tm *wangyonglin_logger_timenow();
int wangyonglin_logger__fileexists(char *filename);
wangyonglin_logger_t logger_t;
int wangyonglin_logger_init(const char *alog, const char *elog)
{

    logger_t.alog = wangyonglin_conf_string(alog);
    logger_t.elog = wangyonglin_conf_string(elog);
    return 0;
}


void wangyonglin_logger_success(const char *format, ...)
{

    if (logger_t.alog.data == NULL && logger_t.alog.data  == "")
    {
        fprintf(stderr, "logs->access_log : not null \n");
        exit(EXIT_FAILURE);
    }

    if (wangyonglin_logger__fileexists(logger_t.alog.data) == 0)
    {
        fprintf(stderr, "it not exists : %s \n", logger_t.alog.data);
        exit(-1);
    }
    else
    {
        int ret = open(logger_t.alog.data, O_RDWR | O_CREAT, 0640);
        if (ret != -1)
        {
            close(ret);
        }
    }

    FILE *fd = fopen(logger_t.alog.data, "a");
    if (fd == NULL)
    {
        fprintf(stderr, "it open fait : %s \n", logger_t.alog.data);
        exit(-1);
    }

    struct tm *t;
    t = wangyonglin_logger_timenow();
    va_list args;
    va_start(args, format);

    fprintf(fd, "%04d-%02d-%02d %02d:%02d:%02d\t", t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    vfprintf(fd, format, args);
    fputc('\r', fd);
    fputc('\n', fd);
    fflush(fd);
    va_end(args);
    fclose(fd);
}

void wangyonglin_logger_failure(const char *format, ...)
{
    if (access(logger_t.elog.data, F_OK) != 0)
    {
        fprintf(stderr, "\tlog error %s %s \n", logger_t.elog.data, strerror(errno));
        return;
    }

    struct tm *t;

    if (wangyonglin_logger__fileexists(logger_t.elog.data) == 0)
    {
        fprintf(stderr, "it not exists : %s \n", logger_t.elog.data);
        exit(-1);
    }
    else
    {
        int ret = open(logger_t.elog.data, O_RDWR | O_CREAT, 0640);
        if (ret != -1)
        {
            close(ret);
        }
    }

    FILE *fd = fopen(logger_t.elog.data, "a+");
    if (fd == NULL)
    {
        fprintf(stderr, "it open fait : %s \n", logger_t.elog.data);
        exit(-1);
    }
    t = wangyonglin_logger_timenow();
    va_list args;
    va_start(args, format);

    fprintf(fd, "%04d-%02d-%02d %02d:%02d:%02d\t", t->tm_year, t->tm_mon, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
    fprintf(fd, "%s\t", "ERROR");
    vfprintf(fd, format, args);
    fputc('\r', fd);
    fputc('\n', fd);
    fflush(fd);
    va_end(args);
    fclose(fd);
}
struct tm *wangyonglin_logger_timenow()
{
    time_t time_seconds = time(0);
    struct tm *now_time = localtime(&time_seconds);
    now_time->tm_year += 1900;
    now_time->tm_mon += 1;
    return now_time;
}
int wangyonglin_logger__fileexists(char *filename)
{
    return (access(filename, 0) == 0);
}
