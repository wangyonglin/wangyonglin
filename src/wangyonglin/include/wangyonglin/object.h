#if !defined(INCLUDE_WANGYONGLIN_OBJECT_H)
#define INCLUDE_WANGYONGLIN_OBJECT_H
#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#define object_string_command(keystring, valueaddr) \
    {                                               \
        keystring, STRING, valueaddr                \
    }
#define object_integer_command(keystring, valueaddr) \
    {                                                \
        keystring, INTEGER, valueaddr                \
    }
#define object_boolean_command(keystring, valueaddr) \
    {                                                \
        keystring, BOOLEAN, valueaddr                \
    }
#define object_null_command \
    {                         \
        NULL, INVALID, 0      \
    }

typedef struct _object_command_t
{
    const char *keystring;
    type_t datatype;
    size_t addr;
} object_command_t;

void *object_create(void **obj, size_t objsize);
void object_delete(void *obj);
ok_t object_mirror(object_command_t objects[], void *obj, string_t confname, const char *section);
#endif