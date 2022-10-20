#if !defined(__SYSTEM_OPTIONS_H__)
#define __SYSTEM_OPTIONS_H__
#include <system/types.h>
#include <system/pool.h>


typedef struct
{
    char *ini_filename;
    char *pid_filename;
    char *log_filename;
    bool deamoned;
    on_starting_t started;
} system_options_t;

ok_t config_options_initializing(system_options_t *options, int argc, char *argv[]);

ok_t system_options_initializing(allocate_pool_t *pool, int argc, char *argv[],system_options_t **options);
#endif