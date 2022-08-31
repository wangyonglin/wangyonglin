#ifndef __CONFIG__H__
#define __CONFIG__H__
#include <unix/unix.h>
#include <unix/conf.h>

typedef struct
{
    char *log_filename;
    bool daemoned;
    pid_t pid_values;
    char *pid_filename;
    conf_t *cf;
} config_t;

config_t *config_init(config_t **config, int argc, char *argv[]);

void config_del(config_t *config);
#endif //!__CONF__H__