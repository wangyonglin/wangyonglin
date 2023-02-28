#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/pool.h>
pool_t *pool = NULL;
struct _options_t *options;
struct _conf_t *cf;
struct _lock_t *lock;
struct _log_t *log;
void sigintHandler(int signal)
{
    printf("this is sigintHandler\n");
    event_loopexit();
}
void __attribute__((destructor)) __exit()
{
    printf("this is exit\n");

    if (pool)
    {
        allocate_delete(pool);
    }
}

void __attribute__((constructor)) init()
{
    printf("this is init\n");
    signal(SIGHUP, sigintHandler);
    signal(SIGINT, sigintHandler);
    signal(SIGALRM, sigintHandler);
}

app_t *application_create(app_t **app, int argc, char *argv[])
{
    if (!(pool = allocate_create(4096)))
    {
        perror("allocate_create failed");
        exit(EXIT_FAILURE);
    }
    if (!((*app) = (struct _app_t *)allocate(pool, sizeof(struct _app_t))))
    {
        perror("allocate app_t failed");
        exit(EXIT_FAILURE);
    }
    (*app)->pool = pool;
    if (!(options = options_create(pool, argc, argv)))
    {
        perror("options_create failed");
        exit(EXIT_FAILURE);
    }
    (*app)->options = options;

    cf = conf_create(pool, options->cfname);
    if (!cf)
    {
        perror("conf_create failed");
        exit(EXIT_FAILURE);
    }
    (*app)->conf = cf;
    lock = lock_create(pool, cf);
    if (!lock)
    {
        perror("lock_create failed");
        exit(EXIT_FAILURE);
    }

    log = log_create(pool, cf);
    if (!log)
    {
        perror("log_create failed");
        exit(EXIT_FAILURE);
    }
    (*app)->log = log;
    if (options->startup == positive)
    {
        locking(lock);
        return (*app);
    }
    else if (options->startup == negative)
    {
        lockexit(lock);
        exit(EXIT_SUCCESS);
    }
    perror("application_create failed");
    exit(EXIT_FAILURE);
    return NULL;
}

void application_delete(app_t *app)
{
    allocate_delete(pool);
}
