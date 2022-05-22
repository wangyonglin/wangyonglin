#include <wangyonglin/linux.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/core.h>
conf_t *conf_new(const char *filename)
{
    size_t len = strlen(filename);
    conf_t *obj = (conf_t *)global_hooks.allocate(sizeof(conf_t));
    if (obj && len > 0 && len < 256)
    {
        memset(obj, 0, sizeof(conf_t));

        obj->filename = strdup(filename);
        if (obj && obj->filename)
        {
            obj->handler = NCONF_new(NULL);
            if (obj->handler && obj->filename)
            {
                long eline = 0;
                if (NCONF_load(obj->handler, obj->filename, &eline) == 1)
                {
                    return obj;
                }
                else
                {
                    NCONF_free(obj->handler);
                    obj->handler = NULL;
                }
            }
        }
    }
    if (obj)
    {
        global_hooks.deallocate(obj);
    }
    return NULL;
}

void conf_free(conf_t *conf)
{
    if (conf)
    {
        if (conf->filename)
        {
            free(conf->filename);
        }
        if (conf->handler)
        {
            NCONF_free(conf->handler);
        }
        free(conf);
    }
}

tags_t conf_get_tags(conf_t *conf, const char *group, const char *name)
{
    if (conf->handler)
    {
        char *out = NCONF_get_string(conf->handler, group, name);
        if (out)
        {
            if ((strcmp(out, "on") == 0) || (strcmp(out, "On") == 0) || (strcmp(out, "ON") == 0))
            {
                return On;
            }
            else if ((strcmp(out, "off") == 0) || (strcmp(out, "Off") == 0) || (strcmp(out, "OFF") == 0))
            {
                return Off;
            }
        }
    }
    return None;
}
char *conf_get_string(conf_t *conf, const char *group, const char *name)
{
    if (conf->handler)
    {
        char *out = NCONF_get_string(conf->handler, group, name);
        int len = strlen(out);
        if (out && len > 0 && len < 250)
        {
            return out;
        }
    }

    return NULL;
}