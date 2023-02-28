#include <wangyonglin/regedit.h>

ok_t regedit(void *obj, struct _pool_t *pool, char *filename, const char *section, struct _regedit_command_t commands[])
{
    if (!obj && !pool && filename)
    {
        return NullPointerException;
    }

    CONF *pConf = NCONF_new(NULL);
    BIO *pBio = BIO_new_file(filename, "r");
    if (pBio == NULL)
    {
        message("加载配置文件失败", filename);
        exit(EXIT_FAILURE);
        return -1;
    }

    long callintger;
    long lELine = 0;
    NCONF_load_bio(pConf, pBio, &lELine);
    void *handler;
    int i = 0;
    while (commands[i].name)
    {
        /* code */
        handler = (obj) + commands[i].address;

        if (commands[i].type == STRING)
        {

            char *out = NCONF_get_string(pConf, section, commands[i].name);
            if (out)
            {
                string_create(pool, handler, out, strlen(out));
            }
            else
            {
                string_create(pool, handler, commands[i].value, strlen(commands[i].value));
            }
        }
        else if (commands[i].type == INTEGER)
        {
            if (NCONF_get_number(pConf, section, commands[i].name, &callintger) == 1)
            {
                integer_create(handler, callintger);
            }
            else
            {
                integer_create(handler, commands[i].value);
            }
        }
        else if (commands[i].type == BOOLEAN)
        {
            char *buffer = NCONF_get_string(pConf, section, commands[i].name);
            if (buffer)
            {

                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {

                    boolean_create(handler, positive);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {

                    boolean_create(handler, negative);
                }
            }
            else
            {
                errno = EINVAL;
                perror(commands[i].name);
                exit(EXIT_FAILURE);
            }
        }
        i++;
    }

    BIO_free(pBio);
    NCONF_free(pConf);
    return OK;
}

boolean toBoolean(char *objstr)
{
    if (objstr)
    {
        if ((strcmp(objstr, "on") == 0) || (strcmp(objstr, "ON") == 0) || strcmp(objstr, "On") == 0)
        {
            return positive;
        }
        else if ((strcmp(objstr, "off") == 0) || (strcmp(objstr, "OFF") == 0) || (strcmp(objstr, "Off") == 0))
        {
            return negative;
        }
    }
    return invalid;
}
