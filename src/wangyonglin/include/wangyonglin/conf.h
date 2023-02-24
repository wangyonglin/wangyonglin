#if !defined(INCLUDE_WANGYONGLIN_CONF_H)
#define INCLUDE_WANGYONGLIN_CONF_H

#include <wangyonglin/package.h>
#include <wangyonglin/string.h>


#define conf_command_size(array) ((sizeof(array)) / (sizeof(array[0])))

typedef struct _conf_command
{
    const char *name;
    void *value;
    character type;
    size_t address;
} conf_command;

ok_t conf_create(void *pointer,const char *filename, const char *section, conf_command commands[], int commands_size);
void conf_delete(void *pointer);
#endif