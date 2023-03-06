#include <wangyonglin/buffer.h>
#include <wangyonglin/string.h>
boolean buffer_is_null(char *outbuffer)
{
    if (!outbuffer)
        return negative;
    else
        return positive;
}
boolean buffer_is_empty(char *outbuffer)
{
    if (outbuffer)
    {
        if (strcmp(outbuffer, "") == 0)
            return positive;
    }
    return negative;
}

char *buffer_create(char **outstring, char *datastring, size_t datalength)
{

    if (datastring)
    {
        if ((*outstring) = global_hooks.allocate(datalength + 1))
        {
            memset((*outstring), 0x00, datalength + 1);
            return memcpy((*outstring), datastring, datalength);
        }
    }
    else
    {
        if ((*outstring) = global_hooks.allocate(datalength))
        {
            memset((*outstring), 0x00, datalength);
            return (*outstring);
        }
    }
    return (*outstring) = NULL;
}
void buffer_delete(char *datastring)
{
    if (datastring)
        global_hooks.deallocate(datastring);
}
void buffer_print(const char *title, char *text)
{
    printf("\t%s:[%d]\t%s\r\n", title, strlen(text), text);
}
char *buffer_max_create(char **formerString, size_t formerStringMax, char *datastring, size_t datalength)
{
    if ((*formerString) = global_hooks.allocate(formerStringMax))
    {

        if (datastring != NULL)
        {
            memset((*formerString), 0x00, formerStringMax);
            memcpy((*formerString), datastring, datalength);
            return (*formerString);
        }
        else
        {
            memset((*formerString), 0x00, formerStringMax);
            return (*formerString);
        }
    }
    return (*formerString) = NULL;
}