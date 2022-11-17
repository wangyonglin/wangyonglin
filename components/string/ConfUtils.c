#include <ConfUtils.h>
void *ConfUtils_toString(void **pointer, void *data, int data_size);
void ConfUtils_toInteger(void **pointer, long data);
void *ConfUtils_toBoolean(void **pointer, bool data);

ok_t ConfUtils_initializing(ConfUtils_t **ConfUtils, AllocateUtils_t *AllocateUtils, const char *filename)
{
    if (!AllocateUtils && !filename)
    {
        return ArgumentException;
    }
    (*ConfUtils) = AllocateUtils_pool(AllocateUtils, sizeof(ConfUtils_t));
    if (!(*ConfUtils))
    {
        return NullPointerException;
    }
    (*ConfUtils)->filename = strdup(filename);
    (*ConfUtils)->AllocateUtils = AllocateUtils;
    return OK;
}
void ConfUtils_clean(ConfUtils_t *ConfUtils, AllocateUtils_t *AllocateUtils)
{

    if (ConfUtils)
    {
        AllocateUtils_free(AllocateUtils, ConfUtils);
    }
}
ok_t ConfUtils_final(ConfUtils_t *ConfUtils, void **pointer, int pointer_size, const char *group, ConfUtils_command_t ConfUtils_commands[], int ConfUtils_commands_size)
{
    if (!ConfUtils->filename)
    {
        return ArgumentException;
    }

    CONF *conf;
    long errline;
    if (!(conf = NCONF_new(NULL)))
    {
        return ErrorException;
    }
    if (NCONF_load(conf, ConfUtils->filename, &errline) < 0)
    {
        return ErrorException;
    }
    AllocateUtils_t *AllocateUtils = ConfUtils->AllocateUtils;
    if (!AllocateUtils)
    {
        return ErrorException;
    }
    (*pointer) = AllocateUtils_pool(AllocateUtils, pointer_size);
    if (!(*pointer))
    {
        return NullPointerException;
    }

    long number_buffer;

    for (int i = 0; i < ConfUtils_commands_size; i++) // 排序要排 元素总个数-1，最后一个元素不用排
    {

        if (ConfUtils_commands[i].type == string)
        {
            char *buffer = NCONF_get_string(conf, group, ConfUtils_commands[i].data);
            if (buffer)
            {
                ConfUtils_toString(&(**pointer) + ConfUtils_commands[i].address, buffer, strlen(buffer));
            }
        }
        else if (ConfUtils_commands[i].type == integer)
        {
            if (NCONF_get_number(conf, group, ConfUtils_commands[i].data, &number_buffer) == 1)
            {
                ConfUtils_toInteger(&(**pointer) + ConfUtils_commands[i].address, number_buffer);
            }
        }
        else if (ConfUtils_commands[i].type == boolean)
        {
            char *buffer = NCONF_get_string(conf, group, ConfUtils_commands[i].data);
            if (buffer)
            {
                
                if (strcmp(buffer, "true") == 0)
                {
                    ConfUtils_toBoolean(&(**pointer) + ConfUtils_commands[i].address, true);
                }
                else if (strcmp(buffer, "false") == 0)
                {
                    ConfUtils_toBoolean(&(**pointer) + ConfUtils_commands[i].address, false);
                }
            }
        }
    }
    NCONF_free(conf);
    return OK;
}
void ConfUtils_final_clean(ConfUtils_t *ConfUtils, void *pointer)
{
    AllocateUtils_free(ConfUtils->AllocateUtils, pointer);
}
void ConfUtils_toInteger(void **pointer, long data)
{
    *pointer = (void*)data;
}
void *ConfUtils_toString(void **pointer, void *data, int data_size)
{
    if ((*pointer) = malloc(data_size + 1))
    {
        memset((*pointer), 0x00, data_size + 1);
        memcpy((*pointer), data, data_size);
        return *pointer;
    }
    return NULL;
}

void *ConfUtils_toBoolean(void **pointer, bool data)
{
    *pointer = (void*)((long)data);
    return NULL;
}