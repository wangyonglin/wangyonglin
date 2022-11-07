#include <StringUtils.h>
#include <string.h>
char *StringCreate(char **pointer, char *data, int length)
{
    if ((*pointer) = (char *)string_hooks.allocate(length + 1))
    {
        if (memset((*pointer), 0x00, length))
        {
            if (memcpy((*pointer), data, length))
            {
                return (*pointer);
            }
        }
        string_hooks.deallocate((*pointer));
        (*pointer) = NULL;
    }
    return NULL;
}

void StringDelect(char *pointer)
{
    if (pointer != NULL)
    {
        string_hooks.deallocate(pointer);
        pointer = NULL;
    }
}

char *StringAllocate(char **pointer, size_t size)
{

    if ((*pointer) = (char *)string_hooks.allocate(size))
    {
        if (memset((*pointer), 0x00, size))
        {
            return (*pointer);
        }
    }
    return NULL;
}
char *StringCopy(char *pointer, char *data, int length)
{
    if (pointer && data )
    {
        if (memcpy(pointer, data, length))
        {
            return (pointer);
        }
    }
    return NULL;
}
void StringDeallocate(char *pointer)
{
    if (pointer != NULL)
    {
        string_hooks.deallocate(pointer);
        pointer = NULL;
    }
}