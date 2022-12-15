#if !defined(DEVELOPER_CONF_H)
#define DEVELOPER_CONF_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <developer/public.h>
#include <developer/allocate.h>

typedef enum
{
    String,
    Integer,
    Boolean
} conf_type_t;

typedef struct
{
    const char *data;
    conf_type_t type;
    size_t address;
} conf_command_t;

typedef struct
{
    unsigned char *filename;
    allocate_t *allocate;
} conf_t;

ok_t conf_create(conf_t **conf, allocate_t *allocate, const char *filename);
ok_t conf_mapping(conf_t *conf, void **pointer, int pointer_size, const char *group, conf_command_t conf_commands[], int conf_commands_size);
size_t conf_commands_size(conf_command_t commands[]);

#endif