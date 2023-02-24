#include <wangyonglin/application.h>
#include <wangyonglin/pool.h>
pool_t *pool = NULL;
void __attribute__((destructor)) __exit()
{
    // printf("this is exit\n");
    exit(EXIT_SUCCESS);
}

void __attribute__((constructor)) init()
{
    // printf("this is init\n");
    signal(SIGHUP, __exit);
    signal(SIGINT, __exit);
    signal(SIGALRM, __exit);
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
    string_rows("ini_file", (*app)->options->ini_file);

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
    logerr((*app)->log, "wangyonglin");
    if ((*app)->options->startup == positive)
    {
        string_rows("app", "start");

        locking((*app)->lock);
        return (*app);
    }
    else if ((*app)->options->startup == negative)
    {
        string_rows("app", "stop");
        lockexit((*app)->lock);
        exit(EXIT_SUCCESS);
    }

    // // string_rows("启动", "失败");
    return NULL;
}

void application_delete(app_t *app)
{
    allocate_delete(pool);
}
