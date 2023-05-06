#if !defined(INCLUDE_MEMORYINJECT_H)
#define INCLUDE_MEMORYINJECT_H

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdarg.h>
#include <Unix_timestamp_converter.h>
#include <Stringex.h>
#include <zlog/zlog.h>
// #define MEMORYINJECT_FILE_COMMAND(keystring, addr)  {keystring, OBJECT_TYPE_FILE, addr}
// #define MEMORYINJECT_STRING_COMMAND(keystring, addr)  {keystring, OBJECT_TYPE_STRING, addr}
// #define MEMORYINJECT_INTEGER_COMMAND(keystring, valueaddr) { keystring, OBJECT_TYPE_INTEGER, valueaddr }
// #define MEMORYINJECT_BOOLEAN_COMMAND(keystring, valueaddr) { keystring, OBJECT_TYPE_BOOLEAN, valueaddr }
// #define MEMORYINJECT_NULL_COMMAND {NULL, OBJECT_TYPE_INVALID, 0}

#define INJECT_COMMAND_FILE(data, addr) \
    {                                   \
        data, OBJECT_TYPE_FILE, addr    \
    }
#define INJECT_COMMAND_STRING(data, addr) \
    {                                     \
        data, OBJECT_TYPE_STRING, addr    \
    }
#define INJECT_COMMAND_INTEGER(data, addr) \
    {                                      \
        data, OBJECT_TYPE_INTEGER, addr    \
    }
#define INJECT_COMMAND_BOOLEAN(data, addr) \
    {                                      \
        data, OBJECT_TYPE_BOOLEAN, addr    \
    }
#define INJECT_COMMAND_NULL          \
    {                                \
        NULL, OBJECT_TYPE_INVALID, 0 \
    }

// typedef struct MemoryInject_Command_s
// {
//     const char *keystring;
//     Integer type;
//     size_t addr;
// } MemoryInject_Command, MemoryInject_Command_t;

typedef struct _InjectCommand
{
    const char *name;
    Integer type;
    size_t addr;
} InjectCommand, InjectCommand_t;

typedef struct _InjectObject
{
    Stringex ini;
    Stringex section;
    zlog_category_t *log;
} InjectObject, InjectObject_t;

Boolean InjectObjectCreate(InjectObject **injectObject, Stringex ini);
Boolean InjectObjectLogger(InjectObject *injectObject, zlog_category_t *log);
void InjectObjectDelect(InjectObject *injectObject);

Boolean InjectCommandInit(InjectObject *injectObject, InjectCommand injectCommand[], void *objectPointer, const char *section);
#endif