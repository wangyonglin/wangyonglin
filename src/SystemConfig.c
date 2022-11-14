#include <SystemConfig.h>
#include <SystemString.h>
#include <SystemPidfile.h>
#include <string.h>
#include <SystemDaemon.h>

#include <SystemOptions.h>
#include <SystemError.h>
#include <AllocateUtils.h>
AllocateUtils_t *__AllocateUtils = NULL;
SystemOptions_t *__SystemOptions = NULL;
SystemLog_t *__SystemLog = NULL;
SystemPidfile_t *__SystemPidfile = NULL;
ConfUtils_t *__ConfUtils = NULL;
ok_t SystemConfig_initializing(SystemConfig_t **SystemConfig, int argc, char *argv[])
{
    ok_t rc;
    if (AllocateUtils_initializing(&__AllocateUtils, 4096) == NULL)
    {
        fprintf(stderr, "SystemAllocate_initializing NullPointerException");
        return NullPointerException;
    }
    if (((*SystemConfig) = AllocateUtils_pool(__AllocateUtils, sizeof(SystemConfig_t))) == NULL)
    {
        return NullPointerException;
    }
    if ((rc = SystemOptions_initializing(&__SystemOptions, __AllocateUtils, argc, argv)) != OK)
    {
        return rc;
    }

    if ((rc = SystemLog_initializing(&__SystemLog, __AllocateUtils, __SystemOptions->log_filename, __SystemOptions->deamoned)) != OK)
    {
        return rc;
    }

    if ((rc = SystemPidfile_initializing(&__SystemPidfile, __AllocateUtils, __SystemOptions->pid_filename)) != OK)
    {
        return rc;
    }

    if (__SystemOptions->started == on_start)
    {
        if (SystemPidfile_listene(__SystemPidfile) == NoneException)
        {

            AllocateUtils_cleanup(__AllocateUtils);
            exit(EXIT_SUCCESS);
        }
        if (__SystemOptions->deamoned == TRUE)
        {
            config_daemon_start();
        }
        if ((rc = SystemPidfile_crt(__SystemPidfile)) != OK)
        {
            return rc;
        }

        if ((rc = ConfUtils_initializing(&__ConfUtils, __AllocateUtils, __SystemOptions->ini_filename)) != OK)
        {
            return rc;
        }
        SystemLog_error(__SystemLog, __SystemOptions->ini_filename);
        if ((*SystemConfig) && __ConfUtils)
        {
            (*SystemConfig)->ConfUtils = __ConfUtils;
        }
        if ((*SystemConfig) && __SystemLog)
        {
            (*SystemConfig)->SystemLog = __SystemLog;
        }
        if ((*SystemConfig) && __AllocateUtils)
        {
            (*SystemConfig)->AllocateUtils = __AllocateUtils;
        }

        return OK;
    }
    else if (__SystemOptions->started == on_stop)
    {

        SystemPidfile_quit(__SystemPidfile);
        AllocateUtils_cleanup(__AllocateUtils);
        exit(EXIT_SUCCESS);
    }

    return ErrorException;
}
void SystemConfig_cleanup(SystemConfig_t *SystemConfig)
{
    // SystemPidfile_del(SystemPidfile);
    AllocateUtils_cleanup(SystemConfig->AllocateUtils);
}