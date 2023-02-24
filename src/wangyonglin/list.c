#include <wangyonglin/list.h>
#include <wangyonglin/string.h>
/**
 * @brief 创建对象列表
 *
 * @param arguments
 * @param items_max
 * @return list*
 */
list *list_create(list **arguments, size_t items_max)
{
    (*arguments) = NULL;
    if ((*arguments) = malloc(sizeof(list)))
    {
        memset((*arguments), 0x00, sizeof(list));
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
void list_delete(list *arguments)
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
int __list_by_name(const void *e1, const void *e2)
{
    return strcmp(((struct _item *)e1)->name, ((struct _item *)e2)->name);
}

/**
 * @brief 字典排序 list
 *
 * @param arguments
 */
void list_sort(list *arguments)
{
    qsort(arguments->items, arguments->items_pos, sizeof(arguments->items[0]), __list_by_name);
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
int list_add_string(list *arguments, const char *name, char *data, size_t datasize)
{
    size_t namesize = strlen(name);
    arguments->items[arguments->items_pos].type = STRING;
    memset(arguments->items[arguments->items_pos].name, 0x00, sizeof(arguments->items[arguments->items_pos].name));
    memcpy(arguments->items[arguments->items_pos].name, name, namesize);
    strcrt((char **)&arguments->items[arguments->items_pos].data, data, datasize);
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
int list_add_integer(list *arguments, const char *name, long data)
{
    size_t namesize = strlen(name);
    arguments->items[arguments->items_pos].type = INTEGER;
    memset(arguments->items[arguments->items_pos].name, 0x00, sizeof(arguments->items[arguments->items_pos].name));
    memcpy(arguments->items[arguments->items_pos].name, name, namesize);
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
int list_add_boolean(list *arguments, const char *name, boolean data)
{
    size_t namesize = strlen(name);
    arguments->items[arguments->items_pos].type = BOOLEAN;
    memset(arguments->items[arguments->items_pos].name, 0x00, sizeof(arguments->items[arguments->items_pos].name));
    memcpy(arguments->items[arguments->items_pos].name, name, namesize);
    boolean_create((unsigned char **)&arguments->items[arguments->items_pos].data, data);
    if (arguments->items[arguments->items_pos].name && arguments->items[arguments->items_pos].data)
    {
        arguments->items_pos++;
    }
    return 0;
}
