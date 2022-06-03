#ifndef __CONFIG__H__
#define __CONFIG__H__
#include <wangyonglin/linux.h>
#include <wangyonglin/core.h>
#include <string/string.h>
#include <wangyonglin/conf.h>

typedef struct
{
    conf_t *conf;
    char * conf_filename;
    char *log_filename;
    levels_t log_tags;
    bool daemoned;
    pid_t pid_values;
    char *pid_filename;

} config_t;

config_t *config_crt(int argc, char *argv[]);
ok_t config_get_string(config_t *config, const char *group, const char *name,char **values);
ok_t config_get_levels(config_t *config, const char *group, const char *name,levels_t *level);
ok_t config_get_number(config_t *config, const char *group, const char *name,long *values);
ok_t application(config_t *config, void *args);
void config_del(config_t *config);
#endif //!__CONF__H__