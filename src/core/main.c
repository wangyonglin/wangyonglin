#include <wangyonglin/linux.h>
#include <wangyonglin/config.h>
#include <wangyonglin/daemon.h>
#include <wangyonglin/log.h>
#include <wangyonglin/pid.h>

int main(int argc, char *argv[])
{

    config_t *config = config_crt(argc, argv);
    if (config && config->conf_filename)
    {
        if (conf_crt(&(config->conf), config->conf_filename))
        {

            if (log_init(config) == ok)
            {

                if (pid_crt(config) == ok)
                {
                    if (config->daemoned == true)
                    {
                        daemond(config);
                    }
                    if (application(config, NULL) == ok)
                    {
                        log_write(config, LOG_INFO, "application ok!");
                    }
                    pid_del(config);
                }
            }
            conf_del(config->conf);
        }

        config_del(config);
    }

    return 0;
}
