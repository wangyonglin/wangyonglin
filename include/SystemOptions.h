#if !defined(__SYSTEM_OPTIONS_H__)
#define __SYSTEM_OPTIONS_H__
#include <SystemTypes.h>
#include <SystemPool.h>


typedef struct
{
    char *ini_filename;
    char *pid_filename;
    char *log_filename;
    bool deamoned;
    on_starting_t started;
} SystemOptions_t;

ok_t SystemOptions_initializing(SystemOptions_t **SystemOptions, allocate_pool_t *pool, int argc, char *argv[]);
ok_t SystemOptions_cleanup(SystemOptions_t *SystemOptions, allocate_pool_t *pool);

#endif