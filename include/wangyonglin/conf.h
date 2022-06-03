#ifndef __WANGYONGLIN_CONF_H__
#define __WANGYONGLIN_CONF_H__
#include <wangyonglin/linux.h>
#include <wangyonglin/core.h>
#include <string/string.h>

typedef struct
{
    CONF *handler;
} conf_t;

conf_t *conf_crt(conf_t **conf, const char *filename);

void conf_del(conf_t *conf);
ok_t conf_get_levels(conf_t *conf, const char *group, const char *name,levels_t* level);
ok_t conf_get_string(conf_t *conf, const char *group, const char *name, char **values);
ok_t conf_get_number(conf_t *conf, const char *group, const char *name, long *values);
void conf_free(conf_t *conf);
#endif /**__WANGYONGLIN_CONF_H__**/
