#include <wangyonglin/conf.h>

ok_t command_init(struct _app_t *app, void *pointer, struct _command commands[], const char *section)
{
    if (!pointer && !app->options->ini_file)
    {
        return NullPointerException;
    }

    CONF *pConf = NCONF_new(NULL);
    BIO *pBio = BIO_new_file(app->options->ini_file, "r");
    if (pBio == NULL)
    {
        message("加载配置文件失败",app->options->ini_file);
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
        handler = (pointer) + commands[i].address;

        if (commands[i].type == STRING)
        {

            char *out = NCONF_get_string(pConf, section, commands[i].name);
            if (out)
            {
                string_create(app->pool, handler,  out, strlen(out));
            }
            else
            {
                 string_create(app->pool, handler, commands[i].value, strlen(commands[i].value));
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
