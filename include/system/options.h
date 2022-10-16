#if !defined(__SYSTEM_OPTIONS_H__)
#define __SYSTEM_OPTIONS_H__
#include <system/types.h>

typedef struct
{
    char *ini_filename;
    char *pid_filename;
    char *log_filename;
    config_bool_t deamoned;
    config_start_t started;
} config_options_t;

config_options_t *config_options_allocate(config_options_t **options);
void config_options_deallocate(config_options_t *options);
ok_t config_options_initializing(config_options_t *options, int argc, char *argv[]);
#endif