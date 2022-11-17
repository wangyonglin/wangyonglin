#if !defined(__SYSTEM_OPTIONS_H__)
#define __SYSTEM_OPTIONS_H__
#include <SystemTypes.h>
#include <AllocateUtils.h>
#include <SystemError.h>
typedef enum{
    onStart,
    onStop,
    onStatus
}onStart_t;

typedef struct
{
    char *ini_filename;
    bool deamoned;
    onStart_t listener;
    AllocateUtils_t *AllocateUtils;
} SystemOptions_t;

ok_t SystemOptions_initializing(SystemOptions_t **SystemOptions,AllocateUtils_t *AllocateUtils, int argc, char *argv[]);
void SystemOptions_clean();
#endif