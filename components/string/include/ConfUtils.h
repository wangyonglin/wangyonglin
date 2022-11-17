#if !defined(__COMPONENTS_STRING_INCLUDE_CONFUTILS_H__)
#define __COMPONENTS_STRING_INCLUDE_CONFUTILS_H__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <PublicUtils.h>

#include <AllocateUtils.h>

#define CONFUTILS_COMMANDS_SIZE(commands) (sizeof(commands) / sizeof(commands[0]))

typedef enum {
    string,
    integer,
    boolean
}type_t;
typedef size_t ConfUtils_offset_t;
typedef struct
{
    char *data;
    type_t type;
    size_t address;
} ConfUtils_command_t;

typedef struct
{
   char * filename;
   AllocateUtils_t *AllocateUtils;
} ConfUtils_t;
ok_t ConfUtils_initializing(ConfUtils_t **ConfUtils, AllocateUtils_t *AllocateUtils, const char *filename);
void ConfUtils_clean();
ok_t ConfUtils_final(ConfUtils_t *ConfUtils, void **pointer, int pointer_size, const char *group, ConfUtils_command_t ConfUtils_commands[], int ConfUtils_commands_size);
void ConfUtils_final_clean(ConfUtils_t *ConfUtils,void *pointer);

#endif