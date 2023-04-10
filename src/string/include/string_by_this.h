#if !defined(STRING_INCLUDE_STRING_BY_THIS_H)
#define STRING_INCLUDE_STRING_BY_THIS_H
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
typedef struct internal_hooks
{
    void *(*allocate)(size_t objsize);
    void (*deallocate)(void *obj);
    void *(*reallocate)(void *obj, size_t objsize);

} internal_hooks;
static internal_hooks global_hooks = {malloc, free, realloc};

#define string_null_command \
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

char *string_create(string_by_t *string, char *datastring, size_t datasize);
void string_delete(string_by_t string);
char *string_setting(string_by_t *string, char *datastring);
void *object_create(void **object, size_t objsize);
void object_delete(void *object);
char *string_reset(string_by_t *string, char *valuestring);
#endif