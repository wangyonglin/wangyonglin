#if !defined(WANGYONGLIN_WANGYONGLIN_H)
#define WANGYONGLIN_WANGYONGLIN_H

#include <wangyonglin/package.h>
#include <wangyonglin/pool.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/options.h>
#include <wangyonglin/lock.h>
#include <wangyonglin/daemonize.h>
#include <wangyonglin/log.h>
#include <wangyonglin/message.h>
#include <wangyonglin/string.h>
#include <wangyonglin/conf.h>
typedef struct _app_t
{
    pool_t *pool;
    options_t *options;
    log_t *log;
    conf_t *conf;
    lock_t *lock;
    message_id_t msg1;
    message_id_t msg2;
    message_id_t msg3;
    message_id_t msg4;

} app_t;

app_t *application_create(app_t **app, int argc, char *argv[]);
void application_delete(app_t *app);
#endif