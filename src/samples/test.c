#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
typedef struct _item
{
    char *name;
    void *data;
    int type;
} item;

typedef struct _listitem
{
    struct _item *items;
    int items_pos;
    int items_max;
} listitem;

listitem *listitem_create(listitem **arguments, size_t items_max)
{
    (*arguments) = NULL;
    if ((*arguments) = malloc(sizeof(listitem *)))
    {
        memset((*arguments), 0x00, sizeof(listitem *));
        (*arguments)->items_pos = 0;
        (*arguments)->items_max = items_max;
        ((*arguments)->items) = malloc(sizeof(struct _item *) * items_max);
    }

    return (*arguments);
}
void listitem_delete(listitem *arguments)
{
    if (arguments->items_pos <= 0)
    {
        if (arguments->items)
            free(arguments->items);
        if (arguments)
            free(arguments);
        return;
    }
    if (arguments)
    {
        struct _item *items;
        while (arguments->items_pos > 0)
        {
            items = NULL;
            if (items = &arguments->items[arguments->items_pos])
            {
                if (items->name)
                    free(items->name);
                if (items->data)
                    free(items->data);
                arguments->items_pos--;
            }
        }
        free(arguments->items);
        free(arguments);
    }
}

struct _item *listitem_strcrt(listitem *arguments, const char *name, char *data, size_t datalength)
{
    arguments->items[arguments->items_pos].name = strdup(name);
    if (arguments->items[arguments->items_pos].data = malloc(sizeof(char) * datalength + 1))
    {
        memset(arguments->items[arguments->items_pos].data, 0x00, sizeof(char) * datalength + 1);
        memcpy(arguments->items[arguments->items_pos].data, data, datalength);
    }
    if (arguments->items[arguments->items_pos].name && arguments->items[arguments->items_pos].data)
    {
        arguments->items_pos++;
    }

    return &arguments->items[arguments->items_pos];
}
int __listitem_by_name(const void *e1, const void *e2)
{
    return strcmp(((struct _item *)e1)->name, ((struct _item *)e2)->name);
}
void listitem_sort(listitem *arguments)
{
    qsort(arguments->items, arguments->items_pos, sizeof(arguments->items[0]), __listitem_by_name);
}

int main(int argc, char *argv[])
{
    listitem *arguments = NULL;
    listitem_create(&arguments, 512);
    listitem_strcrt(arguments, "a", "wangyonglin", 11);
    listitem_strcrt(arguments, "c", "wangyonglin", 11);
    listitem_strcrt(arguments, "b", "wangyonglin", 11);
    listitem_strcrt(arguments, "d", "wangyonglin", 11);
    listitem_strcrt(arguments, "1", "wangyonglin", 11);
    listitem_strcrt(arguments, "3", "wangyonglin", 11);
    listitem_strcrt(arguments, "4", "wangyonglin", 11);
    listitem_strcrt(arguments, "8", "wangyonglin", 11);
    listitem_sort(arguments);
    listitem_delete(arguments);

    return 0;
}