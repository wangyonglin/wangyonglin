#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/pool.h>
#include <wangyonglin/object.h>

lock_t *lock;
options_t *options;
config_t *__config;
log_t *__log;
void sigintHandler(int signal)
{
    event_loopexit();
}

void __attribute__((constructor)) init()
{
    signal(SIGHUP, sigintHandler);
    signal(SIGINT, sigintHandler);
    signal(SIGALRM, sigintHandler);
    log_create(&__log);
    lock_create(&lock, __log);
}
void __attribute__((destructor)) __exit()
{
    log_delete(__log);
    lock_delete(lock);
    options_delete(options);
    object_delete(__config);
}

config_t *config_create(config_t **config, int argc, char *argv[])
{

    if (object_create((void **)config, sizeof(config_t)))
    {
        if (options_create(&options, argc, argv))
        {
            __config = (*config);
            if (options->daemonize == positive && options->startup == positive)
            {
                daemonize();
            }
            if (lock)
            {
                if (options->startup == positive)
                {
                    locking(lock);
                    __config->log = __log;
                    __config->options = options;
                    return __config;
                }
                else if (options->startup == negative)
                {
                    lockexit(lock);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }

    return NULL;
}
