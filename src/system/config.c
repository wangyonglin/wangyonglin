#include <system/config.h>
#include <system/string.h>
#include <system/pidfile.h>
#include <string.h>
#include <system/daemon.h>
#include <system/conf.h>
#include <system/options.h>
#include <system/error.h>
config_t *config_allocate(config_t **config)
{
    if (allocate_object((void **)config, sizeof(config_t)) == NULL)
    {
        err_printf("config_allocate %s", strerror(errno));
        return NULL;
    }

    if (config_options_allocate(&((*config)->options)) == NULL)
    {
        err_printf("config_options_allocate %s", strerror(errno));
        return NULL;
    }

    if (config_pidfile_allocate(&((*config)->pidfile)) == NULL)
    {
        err_printf("config_pidfile_allocate %s", strerror(errno));
        return NULL;
    }

    if (config_log_allocate(&((*config)->log)) == NULL)
    {
        err_printf("config_log_allocate %s", strerror(errno));
        return NULL;
    }

    if (config_conf_allocate(&((*config)->conf)) == NULL)
    {
        err_printf("config_log_allocate %s", strerror(errno));
        return NULL;
    }
    return (*config);
}

void config_deallocate(config_t *config)
{
    config_conf_deallocate(config->conf);
    config_log_deallocate(config->log);
    config_pidfile_deallocate(config->pidfile);
    config_options_deallocate(config->options);
    deallocate_object(config);
}

ok_t config_initializing(config_t *config, int argc, char *argv[])
{
    if (config && config->options)
    {
        if (config_options_initializing(config->options, argc, argv) == OK_SUCCESS)
        {

            if (config_pidfile_initializing(config->pidfile, config->options->pid_filename) == OK_SUCCESS)
            {
                if (config_log_initializing(config->log, config->options->log_filename, config->options->deamoned) == OK_SUCCESS)
                {
                    if (config_conf_initializing(config->conf, config->options->ini_filename) == OK_SUCCESS)
                    {
                        return OK_SUCCESS;
                    }
                }
            }
        }
        return OK_ERROR;
    }
    return OK_NONE;
}

ok_t config_management(config_t *config)
{

    if (config->options->started == start)
    {
        if (config_pidfile_listene(config->pidfile) == FLAG_TRUE)
        {
            return OK_QUIT;
        }
        if (config->options->deamoned == TRUE)
        {
            config_daemon_start();
        }

        if (config_pidfile_crt(config->pidfile) != OK_SUCCESS)
        {
            return OK_QUIT;
        }
        return OK_SUCCESS;
    }
    else if (config->options->started == stop)
    {
        config_pidfile_kill(config->pidfile);
    }
    return OK_NONE;
}