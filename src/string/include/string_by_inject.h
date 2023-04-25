#if !defined(UTILS_INCLUDE_STRING_BY_INJECT_H)
#define UTILS_INCLUDE_STRING_BY_INJECT_H

#include <string_by_this.h>
#include <Stringex.h>
#define inject_string_command(keystring, valueaddr) \
    {                                        \
        keystring, STRING, valueaddr         \
    }
#define inject_integer_command(keystring, valueaddr) \
    {                                         \
        keystring, INTEGER, valueaddr         \
    }
#define inject_boolean_command(keystring, valueaddr) \
    {                                         \
        keystring, BOOLEAN, valueaddr         \
    }
#define inject_null_command     \
    {                    \
        NULL, INVALID, 0 \
    }

typedef struct _inject_command_t
{
    const char *keystring;
    type_by_t type;
    size_t addr;
} inject_command_t;

typedef struct _inject_t
{
    Stringex ini;
    Stringex section;
} inject_t;

inject_t *inject_create(inject_t **inject, Stringex ini);
void inject_delect(inject_t *inject);
int inject_build(inject_t *inject, inject_command_t commands[], void *obj, const char *section);
#endif