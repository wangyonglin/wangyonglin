#include <system/config.h>
#include <system/string.h>
#include <system/pidfile.h>
#include <string.h>
#include <system/daemon.h>
#include <system/conf.h>
#include <system/options.h>
#include <system/error.h>
#include <system/pool.h>

ok_t system_config_initializing(config_t **config, int argc, char *argv[])
{
    if (allocate((void **)config, sizeof(config_t)))
    {
        if (system_allocate_initializing(&(*config)->pool, 4096))
        {
            if (system_options_initializing((*config)->pool, argc, argv, &((*config)->options)) == ok)
            {
                if (system_log_initializing((*config)->pool, (*config)->options->log_filename, (*config)->options->deamoned, &((*config)->log)) == ok)
                {
                    if (system_pidfile_initializing((*config)->pool, (*config)->options->pid_filename, &((*config)->pidfile)) == ok)
                    {
                        if ((*config)->options->started == on_start)
                        {
                            if (system_pidfile_listene((*config)->pidfile) == TRUE)
                            {
                                return ok_quit;
                            }
                            if ((*config)->options->deamoned == TRUE)
                            {
                                config_daemon_start();
                            }
                            if (system_pidfile_crt((*config)->pidfile) == ok)
                            {
                                system_log_error((*config)->log, "pidfile create ok");
                            }
                            return ok;
                        }
                        else if ((*config)->options->started == on_stop)
                        {
                            system_pidfile_del((*config)->pidfile);
                            system_pidfile_kill((*config)->pidfile);
                            return ok_quit;
                        }
                    }
                }
            }
        }
    }
    return ok_err_failure;
}
void system_config_cleanup(config_t *config)
{
    system_deallocate_cleanup(config->pool);
    deallocate(config);
}