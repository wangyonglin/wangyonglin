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

#define MEMORYINJECT_STRING_COMMAND(keystring, addr)  {keystring, OBJECT_TYPE_STRING, addr}
#define MEMORYINJECT_INTEGER_COMMAND(keystring, valueaddr) { keystring, OBJECT_TYPE_INTEGER, valueaddr }

#define MEMORYINJECT_BOOLEAN_COMMAND(keystring, valueaddr) { keystring, OBJECT_TYPE_BOOLEAN, valueaddr }

#define MEMORYINJECT_NULL_COMMAND {NULL, OBJECT_TYPE_INVALID, 0}

typedef struct MemoryInject_Command_s
{
    const char *keystring;
    Integer type;
    size_t addr;
} MemoryInject_Command, MemoryInject_Command_t;

typedef struct MemoryInject_s
{
    Stringex ini;
    Stringex section;
} MemoryInject, MemoryInject_t;

Boolean MemoryInjectCreate(MemoryInject **memoryInject, Stringex ini);
void MemoryInjectDelect(MemoryInject *memoryInject);
Boolean MemoryInjectInster(MemoryInject *memoryInject, MemoryInject_Command commands[], void *obj, const char *section);
#endif