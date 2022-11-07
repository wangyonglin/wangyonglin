#if !defined(__COMPONENTS_STRING_INCLUDE_STRINGUTILS_H__)
#define __COMPONENTS_STRING_INCLUDE_STRINGUTILS_H__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
typedef struct {
    size_t len;
    u_char *data;
} string_data_t;

typedef struct
{
    void *(*allocate)(size_t size);
    void (*deallocate)(void *pointer);
    void *(*reallocate)(void *pointer, size_t size);
} string_hooks_t;

static string_hooks_t string_hooks = {malloc, free, realloc};

char *StringCreate(char **pointer, char *data, int length);
void StringDelect(char *pointer);

char *StringAllocate(char **pointer, size_t size);
char *StringCopy(char *pointer, char *data, int length);
void StringDeallocate(char *pointer);
#endif