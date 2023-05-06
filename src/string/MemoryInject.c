#include <MemoryInject.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <StringexFile.h>
void MemoryInject_BooleanCreate(unsigned char **pointer, Boolean value)
{
    *pointer = (unsigned char *)value;
}

void MemoryInject_IntegerCreate(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}

char *MemoryInject_StringexCreate(char **outstring, char *valuestring)
{
    size_t datalength = strlen(valuestring);
    if ((*outstring) = global_hooks.allocate(datalength + 1))
    {
        memset((*outstring), 0x00, datalength + 1);
        memcpy((*outstring), valuestring, datalength);
        return (*outstring);
    }
    return (*outstring) = NULL;
}

Boolean InjectObjectCreate(InjectObject **injectObject, Stringex ini)
{
    Boolean err = Boolean_false;
    if (ObjectCreate((void **)injectObject, sizeof(InjectObject)))
    {
        memset((*injectObject), 0x00, sizeof(InjectObject));
        (*injectObject)->ini = ini;
        return Boolean_true;
    }
    return err;
}
Boolean InjectObjectLogger(InjectObject *injectObject, zlog_category_t *log)
{
    Boolean err = Boolean_false;
    if (injectObject && log)
    {
        injectObject->log = log;
        return Boolean_true;
    }
    return err;
}
void InjectObjectDelect(InjectObject *injectObject)
{
    if (injectObject)
    {
        ObjectDelete(injectObject);
    }
}

Boolean InjectCommandInit(InjectObject *injectObject, InjectCommand injectCommand[], void *objectPointer, const char *section)
{
    Boolean err = Boolean_false;
    if (!injectObject)
        return err;
    if (!Stringex_IsString(injectObject->ini) && !Stringex_IsEmpty(injectObject->ini))
    {
        return err;
    }
    zlog_category_t *log = injectObject->log;

    CONF *pConf = NCONF_new(NULL);
    BIO *pBio = BIO_new_file(injectObject->ini.valuestring, "r");
    if (pBio == NULL)
    {
        if (log)
            zlog_error(log, "[inject_build]=> %s not open", injectObject->ini.valuestring);
        return err;
    }

    long callintger;
    long lELine = 0;
    NCONF_load_bio(pConf, pBio, &lELine);
    int i = 0;

    while (injectCommand[i].name != NULL)
    {
        injectCommand[i].addr += (objectPointer);

        if (injectCommand[i].type == OBJECT_TYPE_FILE)
        {
            char *out = NCONF_get_string(pConf, section, injectCommand[i].name);
            if (out)
            {
                if (log)
                    zlog_info(log, "[配置文件获取成功[%s=>%s]值为{%s}", section, injectCommand[i].name, out);
                StringexFileDataCallback(injectCommand[i].addr, out);
            }
            else
            {
                if (log)
                    zlog_error(log, "[配置文件获取失败[%s=>%s]", section, injectCommand[i].name);
            }
        }
        else if (injectCommand[i].type == OBJECT_TYPE_STRING)
        {

            char *out = NCONF_get_string(pConf, section, injectCommand[i].name);
            if (out)
            {
                if (log)
                    zlog_info(log, "[配置文件获取成功[%s=>%s]值为{%s}", section, injectCommand[i].name, out);
                StringexCreate((Stringex *)injectCommand[i].addr, out, strlen(out));
            }
        }
        else if (injectCommand[i].type == OBJECT_TYPE_INTEGER)
        {
            if (NCONF_get_number(pConf, section, injectCommand[i].name, &callintger) == 1)
            {
                if (log)
                    zlog_info(log, "[配置文件获取成功[%s=>%s]值为{%lld}", section, injectCommand[i].name, callintger);
                MemoryInject_IntegerCreate(injectCommand[i].addr, callintger);
            }
        }
        else if (injectCommand[i].type == OBJECT_TYPE_BOOLEAN)
        {
            char *buffer = NCONF_get_string(pConf, section, injectCommand[i].name);
            if (buffer)
            {

                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {
                    if (log)
                        zlog_info(log, "[配置文件获取成功[%s=>%s]值为{true}", section, injectCommand[i].name);
                    MemoryInject_BooleanCreate(injectCommand[i].addr, Boolean_true);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {
                    if (log)
                        zlog_info(log, "[配置文件获取成功[%s=>%s]值为{%false}", section, injectCommand[i].name);
                    MemoryInject_BooleanCreate(injectCommand[i].addr, Boolean_false);
                }
            }
            else
            {
                MemoryInject_BooleanCreate(injectCommand[i].addr, Boolean_invalid);
            }
        }
        i++;
    }
    BIO_free(pBio);
    NCONF_free(pConf);
    return Boolean_true;
}
