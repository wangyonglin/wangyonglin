#include <system/conf.h>
#include <system/string.h>
#include <system/error.h>
/*设置输出颜色*/
#define red_color "\E[1;31m"
#define color_suffix "\E[0m"
config_conf_t *config_conf_allocate(config_conf_t **conf)
{
    if (allocate_object((void **)conf, sizeof(config_conf_t)))
    {
        allocate_string(&((*conf)->conffile), 512);
        return (*conf);
    }
    return NULL;
}
void config_conf_deallocate(config_conf_t *conf)
{

    deallocate_string(conf->conffile);
    deallocate_object(conf);
}
ok_t config_conf_initializing(config_conf_t *conf, const char *filename)
{
    if (conf)
    {
        setting_string(conf->conffile, filename, strlen(filename));
        if (conf->conf = NCONF_new(NULL))
        {
            if (NCONF_load(conf->conf, conf->conffile, &(conf->errline)) > 0)
            {
                return OK_SUCCESS;
            }
            NCONF_free(conf->conf);
        }
    }
    return OK_ERROR;
}
