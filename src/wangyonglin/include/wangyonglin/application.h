#if !defined(WANGYONGLIN_APPLICATION_H)
#define WANGYONGLIN_APPLICATION_H

#include <wangyonglin/package.h>
#include <wangyonglin/pool.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/options.h>
#include <wangyonglin/lock.h>
#include <wangyonglin/daemonize.h>
#include <wangyonglin/log.h>
#include <wangyonglin/message.h>
#include <wangyonglin/string.h>

typedef struct _app_t
{
    pool_t *pool;
    struct _options_t *options;
    struct _lock_t *lock;
    struct _log_t *log;
    message_id_t msg1;
    message_id_t msg2;
    message_id_t msg3;
    message_id_t msg4;

} app_t;

app_t *application_create(app_t **app, int argc, char *argv[]);
void application_delete(app_t *app);

#endif