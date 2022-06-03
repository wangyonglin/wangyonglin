#include <wangyonglin/config.h>
#include <string/string.h>

char *const shortopts = "d::c:s:";

struct option longopts[] = {
    {"deamon", no_argument, NULL, 'd'},
    {"conf", required_argument, NULL, 'c'},
    {0, 0, 0, 0},
};

config_t *config_crt(int argc, char *argv[])
{
    int rc;
    config_t *config = (config_t *)global_hooks.allocate(sizeof(config_t));
    if (config)
    {
        memset(config, 0x00, sizeof(config_t));

        while ((rc = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1)
        {
            switch (rc)
            {
            case 'd':
                if (optarg == NULL)
                    config->daemoned = true;
                break;
            case 'c':
                if (optarg)
                {
                    string_crt(&config->conf_filename, optarg, strlen(optarg));
                }
                break;
            default:
                break;
            }
        }
      
        return config;
    }
    return NULL;
}

void config_del(config_t *config)
{
    if (config)
    {
        global_hooks.deallocate(config);
        config=NULL;
    }
}

ok_t config_get_levels(config_t *config, const char *group, const char *name, levels_t *level)
{
    return conf_get_levels(config->conf, group, name, level);
}
ok_t config_get_string(config_t *config, const char *group, const char *name, char **values)
{
    return conf_get_string(config->conf, group, name, values);
}
ok_t config_get_number(config_t *config, const char *group, const char *name, long *values)
{
    return conf_get_number(config->conf, group, name, values);
}