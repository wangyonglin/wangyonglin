#if !defined(INCLUDE_WANGYONGLIN_OPTIONS_H)
#define INCLUDE_WANGYONGLIN_OPTIONS_H

#include <wangyonglin/package.h>
typedef struct _options_t
{

    datasheet conf;
    boolean startup;
    boolean daemonize;
} options_t;

options_t *options_create(options_t **options, int argc, char *argv[]);

void options_delete(options_t *options);
#endif