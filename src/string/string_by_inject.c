#include <string_by_inject.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
void __boolean_create(unsigned char **pointer, boolean_by_t value)
{
    *pointer = (unsigned char *)value;
}
void __integer_create(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}
char *__string_create(char **outstring, char *valuestring)
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

inject_t *inject_create(inject_t **inject, string_by_t ini)
{
    if (((*inject) = (inject_t *)global_hooks.allocate(sizeof(inject_t))))
    {
        memset((*inject), 0x00, sizeof(inject_t));
        (*inject)->ini = ini;
        return (*inject);
    }
    return NULL;
}
void inject_delect(inject_t *inject)
{
    if (inject)
    {
        global_hooks.deallocate(inject);
    }
}

int inject_build(inject_t *inject, inject_command_t commands[], void *obj, const char *section)
{
    if (!inject)
        return -1;

    CONF *pConf = NCONF_new(NULL);
    BIO *pBio = BIO_new_file(inject->ini.valuestring, "r");
    if (pBio == NULL)
    {
        fprintf(stderr, "[inject_build]=> %s not open", inject->ini.valuestring);
        exit(EXIT_FAILURE);
        return -1;
    }

    long callintger;
    long lELine = 0;
    NCONF_load_bio(pConf, pBio, &lELine);

    int i = 0;

    while (commands[i].keystring != NULL)
    {
        commands[i].addr += (obj);
        if (commands[i].type == STRING)
        {

            char *out = NCONF_get_string(pConf, section, commands[i].keystring);
            if (out)
            {
                string_create(commands[i].addr,out, strlen(out));
            }
        }
        else if (commands[i].type == INTEGER)
        {
            if (NCONF_get_number(pConf, section, commands[i].keystring, &callintger) == 1)
            {
                __integer_create(commands[i].addr, callintger);
            }
        }
        else if (commands[i].type == BOOLEAN)
        {
            char *buffer = NCONF_get_string(pConf, section, commands[i].keystring);
            if (buffer)
            {

                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {

                    __boolean_create(commands[i].addr, positive);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {

                    __boolean_create(commands[i].addr, negative);
                }
            }
        }
        i++;
    }
    BIO_free(pBio);
    NCONF_free(pConf);
    return 0;
}
