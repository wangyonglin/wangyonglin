#if !defined(__SYSTEM_OPTIONS_H__)
#define __SYSTEM_OPTIONS_H__
#include <SystemTypes.h>
#include <SystemAllocate.h>


typedef struct
{
    char *ini_filename;
    char *pid_filename;
    char *log_filename;
    bool deamoned;
    on_starting_t started;
} SystemOptions_t;

ok_t SystemOptions_initializing(SystemOptions_t **SystemOptions,SystemAllocate_t *SystemAllocate, int argc, char *argv[]);
void SystemOptions_clean();
#endif