#include <wangyonglin/elements.h>

elements *elements_create(elements **pointer,size_t element_max)
{
    if ((*pointer) = malloc(sizeof(elements)))
    {
        memset((*pointer), 0x00, sizeof(elements));
        (*pointer)->element_pos = -1;
        ((*pointer)->element_data) = malloc(sizeof(struct _element)*element_max);
        if ((*pointer)->element_data)
        {
            memset((*pointer)->element_data, 0x00, sizeof(struct _element)*element_max);

            return (*pointer);
        }
    }
    return (*pointer);
}
void elements_delete(elements *arguments)
{

    if (arguments)
    {

        for (size_t i = 0; i < arguments->element_pos + 1; i++)
        {

            if (arguments->element_data[i]->name)
            {
                free(arguments->element_data[i]->name);
            }
            if (arguments->element_data[i]->type == STRING)
            {
                free(arguments->element_data[i]->content);
            }
            free(arguments->element_data[i]);
        }
        free(arguments->element_data);
        free(arguments);
    }
}

element *__element_string(element **pointer, char *name, char *content, size_t content_size)
{

    if ((*pointer) = (element *)malloc(sizeof(element)))
    {
        (*pointer)->type = STRING;
        int name_size = strlen(name);
        if ((*pointer)->name = malloc(sizeof(char) * name_size + 1))
        {
            memset((*pointer)->name, 0x00, sizeof(char) * name_size + 1);
            memcpy((*pointer)->name, name, name_size);
        }

        if ((*pointer)->content = malloc(sizeof(char) * content_size + 1))
        {
            memset((*pointer)->content, 0x00, sizeof(char) * content_size + 1);
            memcpy((*pointer)->content, content, content_size);
        }
    }
    return (*pointer);
}

element *__element_integer(element **pointer, char *name, long content)
{
    if ((*pointer) = (element *)malloc(sizeof(element)))
    {
        int name_size = strlen(name);
        if ((*pointer)->name = malloc(sizeof(char) * name_size + 1))
        {
            memset((*pointer)->name, 0x00, sizeof(char) * name_size + 1);
            memcpy((*pointer)->name, name, name_size);
        }
        (*pointer)->type = INTEGER;

        (*pointer)->content = (void *)content;
    }
    return (*pointer);
}

ok_t elements_addstring(elements *pointer, char *name, char *content, size_t content_size)
{
    if (!pointer && !pointer->element_data)
    {
        return ArgumentException;
    }

    element *element_data;
    __element_string(&element_data, name, content, content_size);
    pointer->element_pos = pointer->element_pos + 1;
    pointer->element_data[pointer->element_pos] = element_data;
    printf("\t%d    -   %s {%s}\r\n", pointer->element_pos, pointer->element_data[pointer->element_pos]->name, pointer->element_data[pointer->element_pos]->content);

    return Ok;
}

ok_t elements_addinteger(elements *pointer, char *name, long content)
{
    if (!pointer && !pointer->element_data)
    {
        return ArgumentException;
    }
    element *element_data;
    __element_integer(&element_data, name, content);
    pointer->element_pos = pointer->element_pos + 1;
    pointer->element_data[pointer->element_pos] = element_data;
    printf("\t%d    -   %s {%s}\r\n", pointer->element_pos, pointer->element_data[pointer->element_pos]->name, pointer->element_data[pointer->element_pos]->content);
    return Ok;
}

int __elements_by_name(const void *e1, const void *e2)
{
    return strcmp(((struct _element *)e1)->name, ((struct _element *)e2)->name);
}
size_t elements_sort(elements *arguments, struct _element **element_date)
{
    struct _element arguments_data[arguments->element_pos];
    for (size_t i = 0; i < arguments->element_pos; i++)
    {
        arguments_data[i] = *arguments->element_data[i];
    }
    int element_count = sizeof(arguments_data) / sizeof(arguments_data[0]);
    qsort(arguments_data, element_count, sizeof(arguments_data[0]), __elements_by_name);

    if (*element_date = malloc(sizeof(struct _element *) * element_count))
    {
        memcpy(*element_date, arguments_data, sizeof(arguments_data));
        return element_count;
    }
    return -1;
}

size_t elements_count(elements *arguments)
{

    if (arguments)
    {
        return arguments->element_pos + 1;
    }
    return 0;
}

element *elements_object(elements *arguments, size_t arguments_index)
{
    if (arguments)
    {
        return arguments->element_data[arguments_index];
    }
    return NULL;
}