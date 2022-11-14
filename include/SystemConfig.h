#if !defined(INCLUDE_SYSTEMCONFIG_H)
#define INCLUDE_SYSTEMCONFIG_H
#include <SystemTypes.h>

#include <SystemLog.h>
#include <SystemPidfile.h>

#include <SystemOptions.h>
#include <AllocateUtils.h>
#include <ConfUtils.h>
typedef int config_error_t;
typedef struct
{
    SystemLog_t *SystemLog;
    ConfUtils_t *ConfUtils;
    AllocateUtils_t *AllocateUtils;
} SystemConfig_t;

SystemConfig_t *config_allocate(SystemConfig_t **config);
void config_deallocate(SystemConfig_t *config);
// ok_t config_initializing(SystemConfig_t *config, int argc, char *argv[]);
ok_t config_management(SystemConfig_t *config);
ok_t config_destory(SystemConfig_t *config);

ok_t SystemConfig_initializing(SystemConfig_t **config, int argc, char *argv[]);
void SystemConfig_cleanup(SystemConfig_t *config);
#endif