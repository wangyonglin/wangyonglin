#include <wangyonglin/object.h>
#include <wangyonglin/string.h>
#include <wangyonglin/buffer.h>
void __boolean_create(unsigned char **pointer, boolean value)
{
    *pointer = (unsigned char *)value;
}
void __integer_create(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}
char *__string_create(char **outstring, char *datastring)
{
    size_t datalength = strlen(datastring);
    if ((*outstring) = global_hooks.allocate(datalength + 1))
    {
        memset((*outstring), 0x00, datalength + 1);
        memcpy((*outstring), datastring, datalength);
        return (*outstring);
    }
    return (*outstring) = NULL;
}

void *object_create(void **obj, size_t objsize)
{

    if ((*obj) = global_hooks.allocate(objsize))
    {
        //  memset((*obj), 0x00, objsize);
        return (*obj);
    }
    return NULL;
}

void object_delete(void *obj)
{
    if (obj)
        global_hooks.deallocate(obj);
}
/** objects[i].dataaddr += (obj);
        printf("\t[%s=>%p]\r\n", objects[i].keystring, objects[i].dataaddr);
        __string_create(objects[i].dataaddr, objects[i].keystring);*/
ok_t object_mirror(object_command_t objects[], void *obj, datasheet cfg, const char *section)
{
    if (!obj)
    {
        return NullPointerException;
    }
    CONF *pConf = NCONF_new(NULL);
    BIO *pBio = BIO_new_file(datasheet_value(cfg), "r");
    if (pBio == NULL)
    {
        message("加载配置文件失败", datasheet_value(cfg));
        exit(EXIT_FAILURE);
        return -1;
    }

    long callintger;
    long lELine = 0;
    NCONF_load_bio(pConf, pBio, &lELine);
    int i = 0;

    while (objects[i].keystring != NULL)
    {
        objects[i].addr += (obj);
        if (objects[i].datatype == STRING)
        {

            char *out = NCONF_get_string(pConf, section, objects[i].keystring);
            if (out)
            {

                __string_create(objects[i].addr, out);
            }
        }
        else if (objects[i].datatype == INTEGER)
        {
            if (NCONF_get_number(pConf, section, objects[i].keystring, &callintger) == 1)
            {
                __integer_create(objects[i].addr, callintger);
            }
        }
        else if (objects[i].datatype == BOOLEAN)
        {
            char *buffer = NCONF_get_string(pConf, section, objects[i].keystring);
            if (buffer)
            {

                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {

                    __boolean_create(objects[i].addr, positive);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {

                    __boolean_create(objects[i].addr, negative);
                }
            }
        }
        i++;
    }
    BIO_free(pBio);
    NCONF_free(pConf);
    return OK;
}
