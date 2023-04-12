#include <string_by_this.h>

char *string_create(string_by_t *datastring, char *valuestring, size_t valuelength)
{

    if (valuestring)
    {
        if (datastring->valuestring)
        {
            if ((datastring->valuestring = global_hooks.reallocate(datastring->valuestring, valuelength + 1)))
            {
                memset(datastring->valuestring, 0x00, valuelength + 1);
                memcpy(datastring->valuestring, valuestring, valuelength);
                datastring->valuelength = strlen(datastring->valuestring);
                return datastring->valuestring;
            }
        }
        else
        {
            if ((datastring->valuestring = global_hooks.allocate(valuelength + 1)))
            {
                memset(datastring->valuestring, 0x00, valuelength + 1);
                memcpy(datastring->valuestring, valuestring, valuelength);
                datastring->valuelength = strlen(datastring->valuestring);
                return datastring->valuestring;
            }
        }
    }
    else
    {
        datastring->valuestring = NULL;
        datastring->valuelength = 0;
    }
    return datastring->valuestring;
}

void string_delete(string_by_t string)
{
    if (string.valuestring)
        global_hooks.deallocate(string.valuestring);
    string.valuelength = 0;
    string.valuestring = NULL;
}
char *string_updata(string_by_t *datastring, char *valuestring, size_t valuelength)
{

    if (valuestring && datastring->valuestring)
    {

        if ((datastring->valuestring = global_hooks.reallocate(datastring->valuestring, valuelength + 1)))
        {
            memset(datastring->valuestring, 0x00, valuelength + 1);
            memcpy(datastring->valuestring, valuestring, valuelength);

            return datastring->valuestring;
        }
    }
    return datastring->valuestring;
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
    return (*object) = NULL;
}
void object_delete(void *object)
{
    if (object)
    {
        global_hooks.deallocate(object);
    }
}