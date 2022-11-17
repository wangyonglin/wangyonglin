#if !defined(INCLUDE_SYSTEMDAEMON_H)
#define INCLUDE_SYSTEMDAEMON_H
#include <SystemLog.h>
#include <SystemTypes.h>
#include <AllocateUtils.h>
#include <ConfUtils.h>

typedef struct
{
    bool deamoned;
    AllocateUtils_t *AllocateUtils;
} SystemDaemon_t;

ok_t SystemDaemon_initializing(SystemDaemon_t **SystemDaemon, AllocateUtils_t *AllocateUtils, ConfUtils_t *ConfUtils);
ok_t SystemDaemon_start(SystemDaemon_t *SystemDaemon);
#endif