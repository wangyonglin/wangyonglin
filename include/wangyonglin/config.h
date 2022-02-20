#ifndef __CONFIG__H__
#define __CONFIG__H__
#include <wangyonglin/linux.h>
#include <wangyonglin/log.h>
#include <wangyonglin/conf.h>

typedef struct log_s log_t;
typedef struct config_s config_t;

struct config_s
{
    /* data */
    char *log_filename;
    int log_mode;
    FILE *log_fptr;
    char *log_timestamp_format;
};
int config_init(config_t **config, const char *fileconf);
void config_free(config_t *config);
#endif //!__CONFIG__H__