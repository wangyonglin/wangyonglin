#ifndef __WANGYONGLIN_CONF_H__
#define __WANGYONGLIN_CONF_H__
#include <wangyonglin/linux.h>
#include <wangyonglin/core.h>
typedef struct
{
    CONF *handler;
    char *filename;
} conf_t;

conf_t *conf_new(const char *filename);

void conf_free(conf_t *conf);
tags_t conf_get_tags(conf_t *conf, const char *group, const char *name);
char *conf_get_string(conf_t *conf, const char *group, const char *name);

void conf_free(conf_t *conf);
#endif /**__WANGYONGLIN_CONF_H__**/
