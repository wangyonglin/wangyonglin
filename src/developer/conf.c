#include <wangyonglin/conf.h>

// ok_t conf_create(conf_t **conf)
// {
//     if ((*conf) = global_hooks.allocate(sizeof(conf_t)))
//     {
//         memset((*conf), 0x00, sizeof(conf_t));
//         return OK;
//     }
//     return NullPointerException;
// }

ok_t conf_create(void *pointer, const char *filename, const char *section, conf_command commands[], int commands_size)
{
    if (!pointer)
    {
        return ArgumentException;
    }
    CONF *conf = NULL;
    long errline = -1;
    long callintger;
    void *handler;

    conf = NCONF_new(NULL);
    if (NCONF_load(conf, filename, &errline) <= 0)
    {
        if (errline <= 0)
            fprintf(stderr, "Error processing config file\n");
        else
            fprintf(stderr, "Error on line %ld\n", errline);
        NCONF_free(conf);
        return ErrorException;
    }

    for (int i; i < commands_size; i++)
    {

        handler = (pointer) + commands[i].address;

        if (commands[i].type == STRING)
        {
            char *buffer = NCONF_get_string(conf, section, commands[i].name);
            if (buffer)
            {
                string_create(handler, buffer, strlen(buffer));
            }
            else
            {
                string_create(handler, commands[i].value, strlen(commands[i].value));
            }
        }
        else if (commands[i].type == INTEGER)
        {
            if (NCONF_get_number(conf, section, commands[i].name, &callintger) == 1)
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
            char *buffer = NCONF_get_string(conf, section, commands[i].name);
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
                boolean_create(handler, invalid);
            }
        }
    }

    NCONF_free(conf);
    return OK;
}

void conf_delete(void *pointer)
{

    if (pointer)
    {
        // void *handler;
        // for (int i; i < commands_size; i++)
        // {
        //     //   handler = (pointer) + commands[i].address;
        //     if (commands[i].type == STRING)
        //     {
        //         //  handler = (pointer + commands[i].address);

        //         printf("\thandler[%p]\r\n", commands[i].value);
        //         if (&commands[i].value)
        //         {
        //             free(&commands[i].value);
        //         }
        //     }
        // }
        free(pointer);
    }
}
