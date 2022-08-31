#include <unix/config.h>
#include <unix/conf.h>
#include <string/string.h>



config_t *config_init(config_t **config, int argc, char *argv[])
{

    if ((*config) = (config_t *)global_hooks.allocate(sizeof(config_t)))
    {
        memset((*config), 0x00, sizeof(config_t));
        if (conf_crt(&((*config)->cf), "/home/wangyonglin/github/wangyonglin/conf/tiger.conf"))
        {
          //  conf_get_string((*config)->cf, "PID", "filename", &(*config)->pid_filename);
          char * out;
          //  conf_get_string((*config)->cf, "LOG", "filename", &out);
            printf(out);
            return (*config);
        }
    }
    return NULL;
}
void config_del(config_t *config)
{
    if (config)
    {

        string_del(config->log_filename);
        conf_del(config->cf);
    }
}