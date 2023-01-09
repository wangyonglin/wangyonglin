
#if !defined(INCLUDE_WANGYONGLIN_LISTITEM_H)
#define INCLUDE_WANGYONGLIN_LISTITEM_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <developer/public.h>


typedef struct _item
{
    char name[32];
    void *data;
    int type;
} item;

typedef struct _listitem
{
    struct _item *items;
    int items_pos;
} listitem;


/**
 * @brief 创建对象列表
 * 
 * @param arguments 
 * @param items_max 
 * @return listitem* 
 */
listitem *listitem_create(listitem **arguments, size_t items_max);

/**
 * @brief 删除对象列表
 * 
 * @param arguments 
 */
void listitem_delete(listitem *arguments);

/**
 * @brief 排序回调
 * 
 * @param e1 
 * @param e2 
 * @return int 
 */
int __listitem_by_name(const void *e1, const void *e2);

/**
 * @brief 字典排序 listItem
 * 
 * @param arguments 
 */
void listitem_sort(listitem *arguments);

/**
 * @brief 插入字符串为对象列表
 * 
 * @param arguments 
 * @param name 对象名
 * @param data 对象值
 * @param datalength 对象值长度
 * @return int 0 成功 -1 失败
 */
int listitem_add_string(listitem *arguments, const char *name, char *data, size_t datalength);

/**
 * @brief 插入数据类型数据到列表
 * 
 * @param arguments 列表对象
 * @param name 键名
 * @param data long类型
 * @return int  0 成功 -1 失败
 */
int listitem_add_integer(listitem *arguments, const char *name, long data);

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
int listitem_add_boolean(listitem *arguments, const char *name, boolean data);

#endif
