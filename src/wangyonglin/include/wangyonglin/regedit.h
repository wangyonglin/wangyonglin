#if !defined(INCLUDE_WANGYONGLIN_REGEDIT_H)
#define INCLUDE_WANGYONGLIN_REGEDIT_H

#include <wangyonglin/package.h>
#include <wangyonglin/string.h>
#include <wangyonglin/wangyonglin.h>
#define regedit_null_command    \
    {                           \
        NULL, "", INVALID, NULL \
    }

typedef struct _regedit_command_t
{
    const char *name;
    void *value;
    character type;
    size_t address;
} regedit_command_t;

ok_t regedit(void *obj, struct _pool_t *pool, char *filename, const char *section, struct _regedit_command_t commands[]);
#endif