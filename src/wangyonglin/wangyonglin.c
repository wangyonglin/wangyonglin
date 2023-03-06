#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/pool.h>
pool_t *pool = NULL;
// struct _options_t *options;
conf_t *conf;
// struct _lock_t *lock;
// struct _log_t *log;
void sigintHandler(int signal)
{
    event_loopexit();
}
void __attribute__((destructor)) __exit()
{
    if (pool)
        pool_delete(pool);
}

void __attribute__((constructor)) init()
{
    signal(SIGHUP, sigintHandler);
    signal(SIGINT, sigintHandler);
    signal(SIGALRM, sigintHandler);
}

app_t *application_create(app_t **app, int argc, char *argv[])
{
    if ((pool = pool_create(4096)))
    {
        if (pool_object_create(pool, (void **)app, sizeof(struct _app_t)))
        {
            (*app)->pool = pool;
            if (options_create(&(*app)->options, pool, argc, argv))
            {
                if (conf_create(&(*app)->conf, (*app)->pool, (*app)->options->confname))
                {
                    if (log_create(&(*app)->log, (*app)->pool, (*app)->conf))
                    {
                        if (lock_create(&(*app)->lock, (*app)->pool, (*app)->conf))
                        {

                            if ((*app)->options->startup == positive)
                            {
                                locking((*app)->lock);
                                return (*app);
                            }
                            else if ((*app)->options->startup == negative)
                            {
                                lockexit((*app)->lock);
                                exit(EXIT_SUCCESS);
                            }
                            return (*app);
                        }
                    }
                }
            }
        }
    }

    return NULL;
}

void application_delete(app_t *app)
{
    pool_delete(pool);
}
