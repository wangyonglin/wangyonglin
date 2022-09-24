#include <stdio.h>
#include <stdarg.h>
#include <dj/error.h>

void dj_debug(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    fputc('\r', stderr);
    fputc('\n', stderr);
    fflush(stderr);
    va_end(va);
}
void dj_info(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vfprintf(stdout, fmt, va);
    fputc('\r', stdout);
    fputc('\n', stdout);
    fflush(stdout);
    va_end(va);
}