#include <stdio.h>
#include <stdarg.h>
#include <system/error.h>

void err_printf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vfprintf(stderr, fmt, va);
    fputc('\r', stderr);
    fputc('\n', stderr);
    fflush(stderr);
    va_end(va);
}
void error_info(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    vfprintf(stdout, fmt, va);
    fputc('\r', stdout);
    fputc('\n', stdout);
    fflush(stdout);
    va_end(va);
}