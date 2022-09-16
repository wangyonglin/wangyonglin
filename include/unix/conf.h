#if !defined(__UNIX_CONF_H__)
#define __UNIX_CONF_H__
#include <toml/toml.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define disabled ((int)0)
#define enabled ((int)1)

typedef struct
{

    char errbuf[200];
    toml_table_t *conf;
} conf_t;

conf_t *conf_crt(conf_t **conf, const char *filename);

void conf_del(conf_t *conf);
int conf_string_in(conf_t *conf, const char *tablekey, const char *datumkey, char **call);
int conf_int64_in(conf_t *conf, const char *tablekey, const char *datumkey, int64_t *call);
#endif