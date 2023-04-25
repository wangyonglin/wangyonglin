#include <string_by_this.h>

char *string_create(string_by_t *datastring, char *valuestring, size_t valuelength)
{

    if (valuestring && valuelength > 0)
    {
        if ((datastring->valuestring = global_hooks.allocate(valuelength + 1)))
        {
            memset(datastring->valuestring, 0x00, valuelength + 1);
            memcpy(datastring->valuestring, valuestring, valuelength);
            datastring->valuelength = valuelength;
            datastring->valuestring[datastring->valuelength] = '\0';
            return datastring->valuestring;
        }
    }
    return NULL;
}
void string_resetting(string_by_t *datastring, char *valuestring, size_t valuelength)
{
    if (valuestring && valuelength > 0)
    {

        if (datastring->valuestring = global_hooks.reallocate(datastring->valuestring, valuelength + 1))
        {
            memset(datastring->valuestring, 0x00, valuelength + 1);
            memcpy(datastring->valuestring, valuestring, valuelength);
            datastring->valuelength = strlen(datastring->valuestring);
        }
    }
}
void string_delete(string_by_t string)
{
    if (string.valuestring)
        global_hooks.deallocate(string.valuestring);
    string.valuelength = 0;
    string.valuestring = NULL;
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

char *string_slice(string_by_t *datastring, string_by_t deststring, size_t spos, size_t epos)
{
    datastring->valuestring = NULL;
    datastring->valuelength = 0;
    if (deststring.valuestring && spos >= 0 && epos > spos)
    {
        size_t datamaxsize = epos - spos + 1;
        char tmpstring[datamaxsize];
        memset(tmpstring, 0x00, sizeof(tmpstring));
        size_t tmplength = 0;

        if (spos < deststring.valuelength && epos <= deststring.valuelength)
        {
            for (size_t i = 0; i < deststring.valuelength; i++)
            {
                if (spos <= i && epos > i)
                {
                    tmpstring[tmplength++] = deststring.valuestring[i];
                }
            }
            tmpstring[tmplength] = '\0';
        }
        return string_create(datastring, tmpstring, tmplength);
    }
    return datastring->valuestring;
}
