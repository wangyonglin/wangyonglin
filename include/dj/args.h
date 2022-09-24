#if !defined(__DJ_ARGS_H__)
#define __DJ_ARGS_H__
#include <dj/types.h>

#define DJ_ARGS_OK ((dj_int_t)0)
#define DJ_ARGS_NULL ((dj_int_t)-1)
#define DJ_ARGS_ERROR ((dj_int_t)-2)

typedef int dj_started_t;
#define DJ_STARTED_START ((dj_started_t)0)
#define DJ_STARTED_STOP ((dj_started_t)1)
#define DJ_STARTED_NONE ((dj_started_t)2)
typedef struct
{
    dj_char_t *conf;
    dj_bool_t deamon;
    dj_started_t started;
} dj_args_t;

dj_int_t dj_args_init(dj_args_t **args,int argc, char *argv[]);
void dj_args_clean(dj_args_t *args);
#endif