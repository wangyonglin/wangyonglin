#if !defined(INCLUDE_WANGYONGLIN_OPTIONS_H)
#define INCLUDE_WANGYONGLIN_OPTIONS_H
#include <string_by_this.h>
#include <wangyonglin/package.h>
#include <Stringex.h>
typedef struct _options_t
{

    Stringex ini;
    boolean_by_t startup;
    boolean_by_t daemonize;
    Stringex zlog;
} options_t;

 options_t *OptCreate(options_t **options);
void OptDelete(options_t *options);
options_t *OptInit(options_t *options, int argc, char *argv[]);
#endif