#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/pool.h>
pool_t *pool = NULL;
/**
 * 处理SIGINT信号
 **/
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

    if (!object_create(pool, app, sizeof(app_t)))
    {
        perror("allocate app_t failed");
        exit(EXIT_FAILURE);
    }
    (*app)->pool = pool;

    if (!options_create((*app), argc, argv))
    {
        perror("options_create failed");
        exit(EXIT_FAILURE);
    }

    if (!lock_create((*app)))
    {
        perror("lock_create failed");
        exit(EXIT_FAILURE);
    }

    if (!log_create(*app))
    {
        perror("log_create failed");
        exit(EXIT_FAILURE);
    }

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
    perror("application_create failed");
    exit(EXIT_FAILURE);
    return NULL;
}

void application_delete(app_t *app)
{
    allocate_delete(pool);
}
