#if !defined(STRING_INCLUDE_STRING_BY_THIS_H)
#define STRING_INCLUDE_STRING_BY_THIS_H
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef INTERNAL_HOOKS_THIS
#define INTERNAL_HOOKS_THIS
typedef struct internal_hooks
{
    void *(*allocate)(size_t objsize);
    void (*deallocate)(void *obj);
    void *(*reallocate)(void *obj, size_t objsize);

} internal_hooks;
static internal_hooks global_hooks = {malloc, free, realloc};
#endif

#define string_null_command \
    (string_by_t)           \
    {                       \
        0, NULL             \
    }
typedef struct _string_by_t
{
    size_t valuelength;
    char *valuestring;
} string_by_t;

typedef enum _type_by_t
{
    INVALID,
    STRING,
    INTEGER,
    BOOLEAN
} type_by_t;

typedef long boolean_by_t;
#define negative ((boolean_by_t)0)
#define positive ((boolean_by_t)1)
#define invalid ((boolean_by_t)-1)

typedef long integer_by_t;
#define onstart ((integer_by_t)1)
#define onstop ((integer_by_t)0)
#define onstatus ((integer_by_t)-1)
/**
 * @brief 创建字符串对象
 *
 * @param string
 * @param datastring
 * @param datasize
 * @return char*
 */
char *string_create(string_by_t *string, char *datastring, size_t datasize);
/**
 * @brief 删除字符串对象的值和注销指针
 *
 * @param string
 */
void string_delete(string_by_t string);

void *object_create(void **object, size_t objsize);
void object_delete(void *object);
void string_resetting(string_by_t *datastring, char *valuestring, size_t valuelength);
/**
 * @brief 截取字符串deststring 为datastring
 *
 * @param datastring
 * @param deststring
 * @param spos deststring 的起始位置
 * @param epos deststring 的结果位置
 * @return char* datastring->valuestring 的值 失败为NULL
 */
char *string_slice(string_by_t *datastring, string_by_t deststring, size_t spos, size_t epos);

void StringFree(char *datastring);
#endif