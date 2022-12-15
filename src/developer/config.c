#include <developer/config.h>
#include <developer/pidlock.h>
allocate_t *__allocate = NULL;
args_t *args = NULL;
pidlock_t *pidlock = NULL;
log_t *log = NULL;
mapping_t *mapping = NULL;

ok_t config_create(config_t **config, size_t allocate_max_size, int argc, char *argv[])
{
    if (allocate_max_size < 1024)
    {
        perror("allocate_max_size < 1024");
        return ArgumentException;
    }

    if (allocate_initializing(&__allocate, allocate_max_size))
    {
        if (mapping_initializing(&mapping, __allocate) != Ok)
        {
            perror("mapping_initializing failed");
            return ErrorException;
        }
        if (args_initializing(&args, __allocate, argc, argv) != Ok)
        {
            perror("args_initializing failed");
            allocate_cleanup(__allocate);
            return ErrorException;
        }
        printf("\targs->ini_filename%s\r\n", args->ini_filename);
        if (pidlock_initializing(&pidlock, mapping, args->ini_filename) != Ok)
        {
            perror("pidlock_initializing failed");
            allocate_cleanup(__allocate);
            return ErrorException;
        }
        printf("\tpidfile%s\r\n", pidlock->pidfile);
        if (pidlock_status(pidlock) != Ok)
        {
            return ErrorException;
        }

        if (log_initializing(&log, mapping, args->ini_filename, args->daemoned) != Ok)
        {
            perror("log_initializing failed");
            allocate_cleanup(__allocate);
            return ErrorException;
        }
        if (object_create(__allocate, (void **)config, sizeof(config_t)) != Ok)
        {
            perror("object_create->config failed");
            allocate_cleanup(__allocate);
            return ErrorException;
        }
        (*config)->allocate = __allocate;
        (*config)->log = log;
        (*config)->args = args;
        (*config)->mapping = mapping;

        if (args->started == onstart)
        {

            if (pidlock->statused == ENABLED)
            {
                allocate_cleanup(__allocate);
                exit(EXIT_SUCCESS);
            }
            if (args->daemoned == ENABLED)
            {
                daemon_running();
            }
            if (pidlock_locking(pidlock) != Ok)
            {
                allocate_cleanup(__allocate);
                return ErrorException;
            }
            return Ok;
        }
        else if (args->started == onstop)
        {

            pidlock_exit(pidlock);
            allocate_cleanup(__allocate);
            exit(EXIT_SUCCESS);
        }
       
    }
    return ErrorException;
}

void config_destroy(config_t *config)
{
    if (config)
    {
        allocate_cleanup(__allocate);
    }
}

ok_t config_verify(config_t *config)
{
    if (!config)
    {
        perror("pointer not null config");
        return ArgumentException;
    }
    if (!config->allocate)
    {
        perror(" pointer not null config->allocate");
        return ArgumentException;
    }
    if (!config->log)
    {
        perror(" pointer not null config->log");
        return ArgumentException;
    }
    return Ok;
}
