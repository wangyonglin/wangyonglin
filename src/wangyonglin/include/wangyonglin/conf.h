#if !defined(INCLUDE_WANGYONGLIN_CONF_H)
#define INCLUDE_WANGYONGLIN_CONF_H

#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#include <wangyonglin/wangyonglin.h>


typedef struct _command
{
    const char *name;
    void *value;
    character type;
    size_t address;
} command_t;
#define null_command {NULL, "", INVALID, NULL}
ok_t command_init(struct _app_t *app, void *pointer, struct _command commands[], const char *section);

#endif