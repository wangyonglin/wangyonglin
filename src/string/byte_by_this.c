#include <byte_by_this.h>

char *byte_create(char **byte, char *datastring, size_t datasize)
{
    if (((*byte) = global_hooks.allocate(datasize + 1)))
    {
        memset((*byte), 0x00, datasize + 1);
        if (datastring && datasize > 0)
        {
            memcpy((*byte), datastring, datasize);
        }
        return (*byte);
    }
    return NULL;
}
char *byte_copy(char *byte, char *datastring, size_t datasize)
{
    if (byte)
    {
        return memcpy(byte, datastring, datasize);
    }
    return byte;
}
void byte_delete(char *byte)
{
    if (byte)
    {
        global_hooks.deallocate(byte);
    }
}
char *byte_strcat(char *bytes, char *datastring)
{
    if (datastring)
    {
        return strcat(bytes, datastring);
    }
    return NULL;
}