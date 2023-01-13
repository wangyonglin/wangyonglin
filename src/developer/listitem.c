#include <wangyonglin/listitem.h>
#include <wangyonglin/string.h>
/**
 * @brief 创建对象列表
 *
 * @param arguments
 * @param items_max
 * @return listitem*
 */
listitem *listitem_create(listitem **arguments, size_t items_max)
{
    (*arguments) = NULL;
    if ((*arguments) = malloc(sizeof(listitem)))
    {
        memset((*arguments), 0x00, sizeof(listitem));
        (*arguments)->items_pos = 0;
        ((*arguments)->items) = malloc(sizeof(struct _item) * items_max);
    }

    return (*arguments);
}
/**
 * @brief 删除对象列表
 *
 * @param arguments
 */
void listitem_delete(listitem *arguments)
{
    if (arguments)
    {
        if (arguments->items_pos == 0)
        {
            if (arguments->items)
                free(arguments->items);
            if (arguments)
                free(arguments);
            return;
        }
        else
        {
            while (arguments->items_pos > 0)
            {
                arguments->items_pos--;
                if (arguments->items[arguments->items_pos].type == STRING)
                {
                    if (arguments->items[arguments->items_pos].data)
                        free(arguments->items[arguments->items_pos].data);
                }
            }
            free(arguments->items);
            free(arguments);
        }
    }
}
/**
 * @brief 排序回调
 *
 * @param e1
 * @param e2
 * @return int
 */
int __listitem_by_name(const void *e1, const void *e2)
{
    return strcmp(((struct _item *)e1)->name, ((struct _item *)e2)->name);
}

/**
 * @brief 字典排序 listItem
 *
 * @param arguments
 */
void listitem_sort(listitem *arguments)
{
    qsort(arguments->items, arguments->items_pos, sizeof(arguments->items[0]), __listitem_by_name);
}

/**
 * @brief 插入字符串为对象列表
 *
 * @param arguments
 * @param name 对象名
 * @param data 对象值
 * @param datalength 对象值长度
 * @return int 0 成功 -1 失败
 */
int listitem_add_string(listitem *arguments, const char *name, char *data, size_t datalength)
{
    arguments->items[arguments->items_pos].type = STRING;
    memset(arguments->items[arguments->items_pos].name, 0x00, sizeof(arguments->items[arguments->items_pos].name));
    memcpy(arguments->items[arguments->items_pos].name, name, strlen(name));
    strcrt((char **)&arguments->items[arguments->items_pos].data, data, datalength);
    if (arguments->items[arguments->items_pos].name && arguments->items[arguments->items_pos].data)
    {
        arguments->items_pos++;
    }

    return 0;
}
/**
 * @brief 插入数据类型数据到列表
 *
 * @param arguments 列表对象
 * @param name 键名
 * @param data long类型
 * @return int  0 成功 -1 失败
 */
int listitem_add_integer(listitem *arguments, const char *name, long data)
{
    arguments->items[arguments->items_pos].type = INTEGER;
    memset(arguments->items[arguments->items_pos].name, 0x00, sizeof(arguments->items[arguments->items_pos].name));
    memcpy(arguments->items[arguments->items_pos].name, name, strlen(name));
    integer_create((unsigned char **)&arguments->items[arguments->items_pos].data, data);
    if (arguments->items[arguments->items_pos].name && arguments->items[arguments->items_pos].data)
    {
        arguments->items_pos++;
    }

    return 0;
}
/**
 * @brief 插入布尔类型数据到列表
 *
 * @param arguments 列表对象
 * @param name 键名
 * @param data boolean  positive    阳性
 *                      negative    阴性
 *                      invalid     无效
 * @return int  0 成功 -1 失败
 */
int listitem_add_boolean(listitem *arguments, const char *name, boolean data)
{
    arguments->items[arguments->items_pos].type = BOOLEAN;
    memset(arguments->items[arguments->items_pos].name, 0x00, sizeof(arguments->items[arguments->items_pos].name));
    memcpy(arguments->items[arguments->items_pos].name, name, strlen(name));
    boolean_create((unsigned char **)&arguments->items[arguments->items_pos].data, data);
    if (arguments->items[arguments->items_pos].name && arguments->items[arguments->items_pos].data)
    {
        arguments->items_pos++;
    }

    return 0;
}
