#include <wangyonglin/linux.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/core.h>
conf_t *conf_crt(conf_t **conf, const char *filename)
{
    size_t len = strlen(filename);
    *conf = (conf_t *)global_hooks.allocate(sizeof(conf_t));
    if ((*conf) && len > 0 && len < 256)
    {
        memset((*conf), 0, sizeof(conf_t));
        (*conf)->handler = NCONF_new(NULL);
        if ((*conf)->handler && filename)
        {
            long eline = 0;
            if (NCONF_load((*conf)->handler, filename, &eline) == 1)
            {
                return (*conf);
            }
            else
            {
                NCONF_free((*conf)->handler);
                (*conf)->handler = NULL;
            }
        }
    }
    if ((*conf))
    {
        global_hooks.deallocate((*conf));
    }
    return NULL;
}

void conf_del(conf_t *conf)
{
    if (conf)
    {
        if (conf->handler)
        {
            NCONF_free(conf->handler);
        }
        free(conf);
    }
}

ok_t conf_get_levels(conf_t *conf, const char *group, const char *name, levels_t *level)
{
    if (conf->handler)
    {
        char *out = NCONF_get_string(conf->handler, group, name);
        if (out)
        {
            if ((strcmp(out, "on") == 0) || (strcmp(out, "On") == 0) || (strcmp(out, "ON") == 0))
            {
                *level = On;
                return ok;
            }
            else if ((strcmp(out, "off") == 0) || (strcmp(out, "Off") == 0) || (strcmp(out, "OFF") == 0))
            {
                *level = Off;
                return ok;
            }
            else if ((strcmp(out, "none") == 0) || (strcmp(out, "None") == 0) || (strcmp(out, "NONE") == 0))
            {
                *level = None;
                return ok;
            }
        }
    }
    return failed;
}

ok_t conf_get_string(conf_t *conf, const char *group, const char *name, char **values)
{
    if (conf->handler)
    {
        char *out = NCONF_get_string(conf->handler, group, name);
        int len = strlen(out);
        if (out && len > 0 && len < 250)
        {
            (*values) = (char *)global_hooks.allocate(sizeof(char) * len);
            memset((*values), 0x00, sizeof(char) * len);
            strncpy((*values), out, len);
            return ok;
        }
    }

    return failed;
}

ok_t conf_get_number(conf_t *conf, const char *group, const char *name, long *values)
{
    if (NCONF_get_number_e(conf->handler, group, name, values) == 1)
    {
        return ok;
    }
    else
    {
        return failed;
    }
}