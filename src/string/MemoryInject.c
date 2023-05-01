#include <MemoryInject.h>
#include <openssl/conf.h>
#include <openssl/bio.h>

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

Boolean MemoryInjectCreate(MemoryInject **memoryInject, Stringex ini)
{
    Boolean err = Boolean_false;
    if (ObjectCreate((void **)memoryInject, sizeof(MemoryInject)))
    {
        memset((*memoryInject), 0x00, sizeof(MemoryInject));
        (*memoryInject)->ini = ini;
        return Boolean_true;
    }
    return err;
}
void MemoryInjectDelect(MemoryInject *memoryInject)
{
    if (memoryInject)
    {
        ObjectDelete(memoryInject);
    }
}

Boolean MemoryInjectInster(MemoryInject *memoryInject, MemoryInject_Command commands[], void *obj, const char *section)
{
    Boolean err = Boolean_false;
    if (!memoryInject)
        return err;
    if (!Stringex_IsString(memoryInject->ini) && !Stringex_IsEmpty(memoryInject->ini))
    {
        return err;
    }
    CONF *pConf = NCONF_new(NULL);
    BIO *pBio = BIO_new_file(memoryInject->ini.valuestring, "r");
    if (pBio == NULL)
    {
        fprintf(stderr, "[inject_build]=> %s not open", memoryInject->ini.valuestring);
        exit(EXIT_FAILURE);
        return err;
    }

    long callintger;
    long lELine = 0;
    NCONF_load_bio(pConf, pBio, &lELine);

    int i = 0;

    while (commands[i].keystring != NULL)
    {
        commands[i].addr += (obj);
        if (commands[i].type == OBJECT_TYPE_STRING)
        {

            char *out = NCONF_get_string(pConf, section, commands[i].keystring);
            if (out)
            {
                StringexCreate((Stringex *)commands[i].addr, out, strlen(out));
            }
            else
            {
                ((Stringex *)commands[i].addr)->valuelength = 0;
                ((Stringex *)commands[i].addr)->valuestring = NULL;
            }
        }
        else if (commands[i].type == OBJECT_TYPE_INTEGER)
        {
            if (NCONF_get_number(pConf, section, commands[i].keystring, &callintger) == 1)
            {
                MemoryInject_IntegerCreate(commands[i].addr, callintger);
            }
        }
        else if (commands[i].type == OBJECT_TYPE_BOOLEAN)
        {
            char *buffer = NCONF_get_string(pConf, section, commands[i].keystring);
            if (buffer)
            {

                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {

                    MemoryInject_BooleanCreate(commands[i].addr, Boolean_true);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {

                    MemoryInject_BooleanCreate(commands[i].addr, Boolean_false);
                }
            }
            else
            {
                MemoryInject_BooleanCreate(commands[i].addr, Boolean_invalid);
            }
        }
        i++;
    }
    BIO_free(pBio);
    NCONF_free(pConf);
    return Boolean_true;
}
