#if !defined(INCLUDE_WANGYONGLIN_OPTIONS_H)
#define INCLUDE_WANGYONGLIN_OPTIONS_H
#include <string_by_this.h>
#include <wangyonglin/package.h>

typedef struct _options_t
{

    string_by_t conf;
    boolean_by_t startup;
    boolean_by_t daemonize;
} options_t;

options_t *options_create(options_t **options, int argc, char *argv[]);

void options_delete(options_t *options);
#endif