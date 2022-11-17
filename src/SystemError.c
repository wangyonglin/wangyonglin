#include <stdio.h>
#include <stdarg.h>
#include <SystemError.h>
ok_t SystemError_Message(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    fprintf(stderr, "\t");
    fprintf(stderr, fmt, va);
    fprintf(stderr, "\r\n");
    va_end(va);
    return OK;
}
ok_t SystemError_exitMessage(AllocateUtils_t *AllocateUtils, const char *fmt, ...)
{
    if (!AllocateUtils)
    {
        return ArgumentException;
    }
    va_list va;
    va_start(va, fmt);
    fprintf(stderr, fmt, va);
    va_end(va);
    AllocateUtils_cleanup(AllocateUtils);
    exit(EXIT_FAILURE);
}