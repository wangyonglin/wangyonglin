#include <developer/conf.h>


ok_t conf_create(conf_t **conf, allocate_t *allocate, const char *filename)
{
    if (!allocate && !filename)
    {
        return ArgumentException;
    }
    if (object_create(allocate, (void **)conf, sizeof(conf_t)) != Ok)
    {
        return ErrorException;
    }
    if (string_create(allocate, &((*conf)->filename), strdup(filename), strlen(filename)) != Ok)
    {
        return ErrorException;
    }
    (*conf)->allocate = allocate;
    return Ok;
}

ok_t conf_mapping(conf_t *conf, void **pointer, int pointer_size, const char *group, conf_command_t conf_commands[], int conf_commands_size)
{
    if (!conf->filename && !conf->allocate)
    {
        return ArgumentException;
    }

    CONF *openssl_conf;
    long errline;
    if (!(openssl_conf = NCONF_new(NULL)))
    {
        return ErrorException;
    }
    if (NCONF_load(openssl_conf, conf->filename, &errline) < 0)
    {
        return ErrorException;
    }

    if (!(*pointer))
    {
        if (object_create(conf->allocate, (void **)pointer, pointer_size) != Ok)
        {
            return ErrorException;
        }
    }

    long number_buffer;

    for (int i = 0; i < conf_commands_size; i++) // 排序要排 元素总个数-1，最后一个元素不用排
    {

        if (conf_commands[i].type == String)
        {
            char *buffer = NCONF_get_string(openssl_conf, group, conf_commands[i].data);
            if (buffer)
            {

                string_create(conf->allocate, touchar(&(**pointer) + conf_commands[i].address), buffer, strlen(buffer));
            }
        }
        else if (conf_commands[i].type == Integer)
        {
            if (NCONF_get_number(openssl_conf, group, conf_commands[i].data, &number_buffer) == 1)
            {

                number_create(touchar(&(**pointer) + conf_commands[i].address), number_buffer);
            }
        }
        else if (conf_commands[i].type == Boolean)
        {
            char *buffer = NCONF_get_string(openssl_conf, group, conf_commands[i].data);
            if (buffer)
            {
                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {

                    boolean_create(touchar(&(**pointer) + conf_commands[i].address), ENABLED);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {
                    boolean_create(touchar(&(**pointer) + conf_commands[i].address), DISABLED);
                }
            }
        }
    }
    NCONF_free(openssl_conf);
    return Ok;
}
