#include <SystemConf.h>
#include <SystemString.h>
#include <SystemError.h>
/*设置输出颜色*/
#define red_color "\E[1;31m"
#define color_suffix "\E[0m"
ok_t SystemConf_initializing(SystemConf_t **SystemConf, allocate_pool_t *SystemAllocate, const char *filename)
{
    if (((*SystemConf) = SystemAllocate_create(SystemAllocate, sizeof(SystemConf_t))) == NULL)
    {
        return NullPointerException;
    }

    if (SystemString_Pool_initializing(&((*SystemConf)->conffile), SystemAllocate, strdup(filename), strlen(filename)) != OK)
    {
        return ArgumentException;
    }
    if ((*SystemConf)->conf = NCONF_new(NULL))
    {
        if (NCONF_load((*SystemConf)->conf, (*SystemConf)->conffile, &((*SystemConf)->errline)) > 0)
        {
            return OK;
        }
        else
        {
            return ErrorException;
        }
    }

    return NullPointerException;
}
void SystemConf_cleanup(SystemConf_t *SystemConf)
{

    SystemString_cleanup(SystemConf->conffile);
    if (SystemConf->conf)
    {
        NCONF_free(SystemConf->conf);
    }

    deallocate(SystemConf);
}
