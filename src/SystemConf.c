#include <SystemConf.h>
#include <SystemString.h>
#include <SystemError.h>
/*设置输出颜色*/
#define red_color "\E[1;31m"
#define color_suffix "\E[0m"

ok_t SystemConf_initializing(SystemConf_t **SystemConf, SystemAllocate_t *SystemAllocate, const char *filename)
{
    if (!SystemAllocate && !filename)
    {
        return ArgumentException;
    }
    if (((*SystemConf) = SystemAllocate_Create(SystemAllocate, sizeof(SystemConf_t))) == NULL)
    {
        return NullPointerException;
    }

    if ((*SystemConf)->conf = NCONF_new(NULL))
    {
        if (NCONF_load((*SystemConf)->conf, filename, &((*SystemConf)->errline)) > 0)
        {
            (*SystemConf)->SystemAllocate = SystemAllocate;
            return OK;
        }
        else
        {
            return ErrorException;
        }
    }

    return NullPointerException;
}
void SystemConf_clean(SystemConf_t *SystemConf)
{
    if (SystemConf->conf)
    {
        NCONF_free(SystemConf->conf);
    }
}

ok_t SystemConf_String(SystemConf_t *SystemConf, char **pointer, const char *group, const char *name)
{

    if (!SystemConf && !SystemConf->SystemAllocate && !group && !name)
    {
        return ArgumentException;
    }
    char *out = NCONF_get_string(SystemConf->conf, group, name);
    if (out==NULL)
    {
        return ErrorException;
    }
    if (OK != (SystemAllocate_String(pointer, SystemConf->SystemAllocate, out, strlen(out))))
    {
        return NullPointerException;
    }
     printf("%s\t\r\n",*pointer);
    return OK;
}

ok_t SystemConf_Number(SystemConf_t *SystemConf, long *pointer, const char *group, const char *name)
{
    if (!SystemConf && !group && !name)
    {
        return ArgumentException;
    }

    if (NCONF_get_number(SystemConf->conf, group, name, pointer) == 1)
    {
        return OK;
    }
    return ErrorException;
}