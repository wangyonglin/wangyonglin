#include <SystemConfig.h>
#include <SystemString.h>
#include <SystemPidfile.h>
#include <string.h>
#include <SystemDaemon.h>
#include <SystemConf.h>
#include <SystemOptions.h>
#include <SystemError.h>
#include <SystemPool.h>
allocate_pool_t *SystemAllocate = NULL;
SystemOptions_t *SystemOptions = NULL;
SystemLog_t *SystemLog = NULL;
SystemPidfile_t *SystemPidfile = NULL;
SystemConf_t *SystemConf = NULL;
ok_t SystemConfig_initializing(SystemConfig_t **SystemConfig, int argc, char *argv[])
{
    ok_t rc;
    if (SystemAllocate_initializing(&SystemAllocate, 4096) == NULL)
    {
        fprintf(stderr, "SystemAllocate_initializing NullPointerException");
        return NullPointerException;
    }
    if (((*SystemConfig) = SystemAllocate_create(SystemAllocate, sizeof(SystemConfig_t))) == NULL)
    {
        return NullPointerException;
    }
    if ((rc = SystemOptions_initializing(&SystemOptions, SystemAllocate, argc, argv)) != OK)
    {
        return rc;
    }
    if ((rc = SystemLog_initializing(&SystemLog, SystemAllocate, SystemOptions->log_filename, SystemOptions->deamoned)) != OK)
    {
        return rc;
    }
    if ((rc = SystemPidfile_initializing(&SystemPidfile, SystemAllocate, SystemOptions->pid_filename)) != OK)
    {
        return rc;
    }
    if (SystemOptions->started == on_start)
    {
        if (SystemPidfile_listene(SystemPidfile) == NoneException)
        {
            SystemDeallocate_cleanup(SystemAllocate);
            exit(EXIT_SUCCESS);
        }
        if (SystemOptions->deamoned == TRUE)
        {
            config_daemon_start();
        }
        if ((rc = SystemPidfile_crt(SystemPidfile)) != OK)
        {
            return rc;
        }

        if ((rc = SystemConf_initializing(&SystemConf, SystemAllocate, SystemOptions->ini_filename)) != OK)
        {
            printf("\t%d\r\n", rc);
            return rc;
        }
        SystemLog_error(SystemLog, SystemOptions->ini_filename);

        if ((*SystemConfig) && SystemConf)
        {
            (*SystemConfig)->SystemConf = SystemConf;
        }
        if ((*SystemConfig) && SystemLog)
        {
            (*SystemConfig)->SystemLog = SystemLog;
        }
        if ((*SystemConfig) && SystemAllocate)
        {
            (*SystemConfig)->SystemAllocate = SystemAllocate;
        }

        return OK;
    }
    else if (SystemOptions->started == on_stop)
    {

        SystemPidfile_kill(SystemPidfile);
        SystemDeallocate_cleanup(SystemAllocate);
        exit(EXIT_SUCCESS);
    }
    return ErrorException;
}
void SystemConfig_cleanup(SystemConfig_t *SystemConfig)
{
    // SystemPidfile_del(SystemPidfile);
    SystemDeallocate_cleanup(SystemAllocate);
}