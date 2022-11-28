#include <developer/conf.h>
void *stringPointer(void **pointer, void *data, int data_size);
void numberPointer(void **pointer, long data);
void booleanPointer(void **pointer, boolean act);

ok_t conf_initializing(conf_t **conf, allocate_t *allocate, const char *filename)
{
    if (!allocate && !filename)
    {
        return ArgumentException;
    }
    if (object_crt(allocate, (void **)conf, sizeof(conf_t)) != Ok)
    {
        return ErrorException;
    }
    printf("\tfilename %s\r\n", filename);
    if (string_crt(allocate, &((*conf)->filename), strdup(filename), strlen(filename)) != Ok)
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
        if (object_crt(conf->allocate, (void **)pointer, pointer_size) != Ok)
        {
            return ErrorException;
        }
    }

    long number_buffer;

    for (int i = 0; i < conf_commands_size; i++) // 排序要排 元素总个数-1，最后一个元素不用排
    {

        if (conf_commands[i].type == STRING)
        {
            char *buffer = NCONF_get_string(openssl_conf, group, conf_commands[i].data);
            if (buffer)
            {
                stringPointer(&(**pointer) + conf_commands[i].address, buffer, strlen(buffer));
            }
        }
        else if (conf_commands[i].type == NUMBER)
        {
            if (NCONF_get_number(openssl_conf, group, conf_commands[i].data, &number_buffer) == 1)
            {
                numberPointer(&(**pointer) + conf_commands[i].address, number_buffer);
            }
        }
        else if (conf_commands[i].type == BOOLEAN)
        {
            char *buffer = NCONF_get_string(openssl_conf, group, conf_commands[i].data);
            if (buffer)
            {
                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {
                    booleanPointer(&(**pointer) + conf_commands[i].address, enabled);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {
                    booleanPointer(&(**pointer) + conf_commands[i].address, disabled);
                }
            }
        }
    }
    NCONF_free(openssl_conf);
    return Ok;
}

void numberPointer(void **pointer, long data)
{
    *pointer = (void*)data;
}
void *stringPointer(void **pointer, void *data, int data_size)
{
    if ((*pointer) = malloc(data_size + 1))
    {
        memset((*pointer), 0x00, data_size + 1);
        memcpy((*pointer), data, data_size);
        return *pointer;
    }
    return NULL;
}
void booleanPointer(void **pointer, boolean bool)
{
    *pointer = bool;
}