#if !defined(INCLUDE_WANGYONGLIN_CONF_H)
#define INCLUDE_WANGYONGLIN_CONF_H
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <stdbool.h>
#include <stddef.h>
#include <wangyonglin/string.h>


#define conf_command_size(array) ((sizeof(array)) / (sizeof(array[0])))

typedef struct _conf_command
{
    const char *name;
    void *value;
    integer type;
    size_t address;
} conf_command;

ok_t conf_create(void *pointer,const char *filename, const char *section, conf_command commands[], int commands_size);
void conf_delete(void *pointer);
#endif