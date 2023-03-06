
#if !defined(INCLUDE_WANGYONGLIN_LIST_H)
#define INCLUDE_WANGYONGLIN_LIST_H
#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#define list_null_command \
    {                     \
        NULL, NULL, 0, -1 \
    }
#define list_string_command(keystring, valstring) \
    {                                             \
        keystring, valstring, 0, STRING           \
    }
#define list_integer_command(keystring, valinteger) \
    {                                               \
        keystring, NULL, valinteger, INTEGER        \
    }
#define list_count(lists) sizeof(lists) / sizeof(lists[0])
typedef struct _list_t
{
    char *keystring;
    char *valstring;
    size_t valinteger;
    type_t valtype;
} list_t;

void list_sort(struct _list_t lists[], size_t listscount);

#endif
