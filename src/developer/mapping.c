#include <developer/mapping.h>

ok_t mapping_initializing(mapping_t **mapping, allocate_t *allocate)
{
    if (!allocate)
    {
        perror("mapping_initializing pamans allocate null");
        return ArgumentException;
    }
    if (object_create(allocate, (void **)mapping, sizeof(mapping_t)) != Ok)
    {
        perror("mapping_initializing object_create");
        return ErrorException;
    }
    (*mapping)->allocate = allocate;
    return Ok;
}

ok_t mapping_create(mapping_t *mapping, void *pointer, const char *filename, const char *section, mapping_arguments_t arguments[], int mappings_size)
{
    if (!mapping)
    {
        perror("mapping_create pamans mapping null");
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

    for (int i; i < mappings_size; i++)
    {

        handler = touchar(pointer + arguments[i].address);

        if (arguments[i].type == STRING)
        {
            char *buffer = NCONF_get_string(conf, section, arguments[i].name);
            if (buffer)
            {
                string_create(mapping->allocate, handler, buffer, strlen(buffer));
            }
            else
            {
                string_create(mapping->allocate, handler, arguments[i].value, strlen(arguments[i].value));
            }
        }
        else if (arguments[i].type == INTEGER)
        {
            if (NCONF_get_number(conf, section, arguments[i].name, &callintger) == 1)
            {
                integer_setting(handler, callintger);
            }
            else
            {
                integer_setting(handler, arguments[i].value);
            }
        }
        else if (arguments[i].type == BOOLEAN)
        {
            char *buffer = NCONF_get_string(conf, section, arguments[i].name);
            if (buffer)
            {

                if ((strcmp(buffer, "on") == 0) || (strcmp(buffer, "ON") == 0) || strcmp(buffer, "On") == 0)
                {

                    boolean_setting(handler, ENABLED);
                }
                else if ((strcmp(buffer, "off") == 0) || (strcmp(buffer, "OFF") == 0) || (strcmp(buffer, "Off") == 0))
                {

                    boolean_setting(handler, DISABLED);
                }
            }
            else
            {
                boolean_setting(handler, arguments[i].value);
            }
        }
    }

    NCONF_free(conf);
    return Ok;
}

void mapping_delete(void *pointer, mapping_arguments_t arguments[], int arguments_size)
{

    if (pointer)
    {
        void *handler;
        for (int i; i < arguments_size; i++)
        {

            if (arguments[i].type == STRING)
            {
                handler = touchar(pointer + arguments[i].address);
                printf("\thandler[%p]\r\n", handler);
                if (handler)
                {
                    free(handler);
                }
            }
        }
    }
}

int mapping_arguments_qsort_name(const void *e1, const void *e2)
{
    return strcmp(((mapping_arguments_t *)e1)->name, ((mapping_arguments_t *)e2)->name);
}
void mapping_arguments_qsort(mapping_arguments_t *arguments,int arguments_count)
{
    qsort(arguments, arguments_count, sizeof(arguments[0]), mapping_arguments_qsort_name);
}

