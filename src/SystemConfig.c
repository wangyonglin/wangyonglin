#include <SystemConfig.h>
#include <SystemString.h>
#include <SystemPidfile.h>
#include <string.h>
#include <SystemDaemon.h>
#include <SystemConf.h>
#include <SystemOptions.h>
#include <SystemError.h>
#include <SystemAllocate.h>
SystemAllocate_t *__SystemAllocate = NULL;
SystemOptions_t *__SystemOptions = NULL;
SystemLog_t *__SystemLog = NULL;
SystemPidfile_t *__SystemPidfile = NULL;
SystemConf_t *__SystemConf = NULL;
ok_t SystemConfig_initializing(SystemConfig_t **SystemConfig, int argc, char *argv[])
{
    ok_t rc;
    if (SystemAllocate_initializing(&__SystemAllocate, 4096) == NULL)
    {
        fprintf(stderr, "SystemAllocate_initializing NullPointerException");
        return NullPointerException;
    }
    if (((*SystemConfig) = SystemAllocate_Create(__SystemAllocate, sizeof(SystemConfig_t))) == NULL)
    {
        return NullPointerException;
    }
    if ((rc = SystemOptions_initializing(&__SystemOptions, __SystemAllocate, argc, argv)) != OK)
    {
        return rc;
    }
    
    if ((rc = SystemLog_initializing(&__SystemLog, __SystemAllocate, __SystemOptions->log_filename, __SystemOptions->deamoned)) != OK)
    {
        return rc;
    }
    SystemLog_error(__SystemLog,"ok");
   
    if ((rc = SystemPidfile_initializing(&__SystemPidfile, __SystemAllocate, __SystemOptions->pid_filename)) != OK)
    {
        return rc;
    }
     
    if (__SystemOptions->started == on_start)
    {
        if (SystemPidfile_listene(__SystemPidfile) == NoneException)
        {
            SystemDeallocate_cleanup(__SystemAllocate);
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

        if ((rc = SystemConf_initializing(&__SystemConf,__SystemAllocate, __SystemOptions->ini_filename)) != OK)
        {
            printf("\t%d\r\n", rc);
            return rc;
        }
        SystemLog_error(__SystemLog, __SystemOptions->ini_filename);

        if ((*SystemConfig) && __SystemConf)
        {
            (*SystemConfig)->SystemConf = __SystemConf;
        }
        if ((*SystemConfig) && __SystemLog)
        {
            (*SystemConfig)->SystemLog = __SystemLog;
        }
        if ((*SystemConfig) && __SystemAllocate)
        {
            (*SystemConfig)->SystemAllocate = __SystemAllocate;
        }
 
        return OK;
    }
    else if (__SystemOptions->started == on_stop)
    {

        SystemPidfile_quit(__SystemPidfile);
        SystemDeallocate_cleanup(__SystemAllocate);
        exit(EXIT_SUCCESS);
    }
       
    return ErrorException;
}
void SystemConfig_cleanup(SystemConfig_t *SystemConfig)
{
    // SystemPidfile_del(SystemPidfile);
    SystemDeallocate_cleanup(__SystemAllocate);
}