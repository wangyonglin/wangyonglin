#ifndef __CONFIG__H__
#define __CONFIG__H__
#include <wangyonglin/linux.h>
#include <wangyonglin/core.h>
#include <wangyonglin/conf.h>

typedef struct
{
    conf_t *conf;
    char *log_filename;
    tags_t log_tags;
    activated log_activated;
    activated daemon_activated;
} config_t;

config_t *config_new(const char *filename);


void config_free(config_t *config);
#endif //!__CONF__H__