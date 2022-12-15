#include <developer/string.h>
// void *allocate(void **pointer, size_t pointer_size)
// {
//     if ((*pointer) = malloc(pointer_size + 1))
//     {
//         memset((*pointer), 0x00, pointer_size + 1);
//         return (*pointer);
//     }
//     return NULL;
// }

// void deallocate(void *pointer)
// {
//     if (pointer)
//     {
//         free(pointer);
//     }
// }

void *allocate_memcpy_create(void **pointer, void *data, size_t data_size)
{
    if ((*pointer) = malloc(sizeof(data) * data_size + 1))
    {
        memset((*pointer), 0x00, sizeof(data) * data_size + 1);
        memcpy((*pointer), data, data_size);
        return (*pointer);
    }
    return NULL;
}

void boolean_setting(unsigned char **pointer, boolean value)
{
    *pointer = value;
}
void integer_setting(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}

void string_setting(unsigned char **pointer, char *value)
{
    if ((*pointer) = malloc(sizeof(value) * strlen(value) + 1))
    {
        memset((*pointer), 0x00, sizeof(value) * strlen(value) + 1);
        memcpy((*pointer), value, strlen(value));
        return (*pointer);
    }
    return NULL;
}
item_t *items_create(item_t **items, size_t max)
{
    if ((*items) = malloc(sizeof(item_t) * max))
    {
        memset((*items), 0x00, sizeof(item_t) * max);
        return (*items);
    }
}

void items_add(item_t *items, char *name, void *data)
{
    if(items){
        
    }
}
int items_by_name(const void *e1, const void *e2)
{
    return strcmp(((item_t *)e1)->name, ((item_t *)e2)->name);
}

void items_sort(item_t *item, size_t item_count)
{
    qsort(item, item_count, sizeof(item[0]), items_by_name);
}