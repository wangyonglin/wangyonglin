#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#include <wangyonglin/object.h>

datasheet datasheet_create(char *datastring, size_t datalength)
{
    datasheet data = datasheet_null_command;
    if (datastring)
    {

        if ((data.string = global_hooks.allocate(datalength + 1)))
        {
            memset(data.string, 0x00, datalength + 1);
            memcpy(data.string, datastring, datalength);
            data.length = strlen(data.string);
            return data;
        }
    }
    return data;
}
void datasheet_delete(datasheet data)
{
    if (data.string)
        global_hooks.deallocate(data.string);
}
char *datasheet_value(datasheet data)
{
    return data.string;
}
size_t datasheet_length(datasheet data)
{
    return data.length;
}
// package *package_create(package **back)
// {

//     if (object_create((void **)back, sizeof(package)))
//     {
//         (*back)->localdir = datasheet_create(PACKAGE_DIRECTERY_LOCALDIR, strlen(PACKAGE_DIRECTERY_LOCALDIR));
//         (*back)->logdir = datasheet_create(PACKAGE_DIRECTERY_LOG, strlen(PACKAGE_DIRECTERY_LOG));
//         (*back)->confdir = datasheet_create(PACKAGE_DIRECTERY_CONF, strlen(PACKAGE_DIRECTERY_CONF));
//         size_t maxlogdir = 0;

//         return (*back);
//     }
//     return NULL;
// }
// void package_delete(package *back)
// {
//     if (back)
//     {
//         datasheet_delete(back->localdir);
//         datasheet_delete(back->logdir);
//         datasheet_delete(back->confdir);
//         object_delete(back);
//     }
// }