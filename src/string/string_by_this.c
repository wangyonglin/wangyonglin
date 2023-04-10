#include <string_by_this.h>

char *string_create(string_by_t *string, char *valuestring, size_t datasize)
{

    if (valuestring)
    {
        if (string->valuestring)
        {
            if ((string->valuestring = global_hooks.reallocate(string->valuestring, datasize + 1)))
            {
                memset(string->valuestring, 0x00, datasize + 1);
                memcpy(string->valuestring, valuestring, datasize);
                string->valuelength = strlen(string->valuestring);
                return string->valuestring;
            }
        }
        else
        {
            if ((string->valuestring = global_hooks.allocate(datasize + 1)))
            {
                memset(string->valuestring, 0x00, datasize + 1);
                memcpy(string->valuestring, valuestring, datasize);
                string->valuelength = strlen(string->valuestring);
                return string->valuestring;
            }
        }
    }
    return string->valuestring;
}

void string_delete(string_by_t string)
{
    if (string.valuestring)
        global_hooks.deallocate(string.valuestring);
    string.valuelength = 0;
    string.valuestring = NULL;
}
char *string_reset(string_by_t *string, char *valuestring)
{
    string_by_t __tmp = string_null_command;
    if (valuestring)
    {
        size_t datasize = strlen(valuestring);
        if ((__tmp.valuestring = global_hooks.allocate(datasize + 1)))
        {
            memset(__tmp.valuestring, 0x00, datasize + 1);
            memcpy(__tmp.valuestring, valuestring, datasize);
            __tmp.valuelength = strlen(__tmp.valuestring);
            return __tmp.valuestring;
        }
    }
    return __tmp.valuestring;
}
char *string_setting(string_by_t *string, char *valuestring)
{
    string_by_t __tmp = string_null_command;
    if (valuestring)
    {
        size_t datasize = strlen(valuestring);
        if ((__tmp.valuestring = global_hooks.allocate(datasize + 1)))
        {
            memset(__tmp.valuestring, 0x00, datasize + 1);
            memcpy(__tmp.valuestring, valuestring, datasize);
            __tmp.valuelength = strlen(__tmp.valuestring);
            return __tmp.valuestring;
        }
    }
    return __tmp.valuestring;
}
void *object_create(void **object, size_t objsize)
{

    if (((*object) = global_hooks.allocate(objsize)))
    {
        memset((*object), 0x00, objsize);
        return (*object);
    }
    return NULL;
}
void object_delete(void *object)
{
    if (object)
    {
        global_hooks.deallocate(object);
    }
}