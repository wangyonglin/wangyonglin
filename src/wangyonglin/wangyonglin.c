#include <wangyonglin/wangyonglin.h>
#include <curl/curl.h>
lock_t *__lock;
options_t *options;
config_t *__config;
log_t *__log;
inject_t *__inject;
void sigintHandler(int signal)
{
    switch (signal)
    {
    case SIGTERM:
    case SIGHUP:
    case SIGQUIT:
    case SIGINT:
        event_loopbreak(); // 终止侦听event_dispatch()的事件侦听循环，执行以后的代码
        break;
    }
    exit(-1);
}

void __attribute__((constructor)) init()
{
    // 自定义信号处理函数
    signal(SIGHUP, sigintHandler);
    signal(SIGTERM, sigintHandler);
    signal(SIGINT, sigintHandler);
    signal(SIGQUIT, sigintHandler);

    log_create(&__log);
    lock_create(&__lock, __log);
    curl_global_init(CURL_GLOBAL_DEFAULT);
}
void __attribute__((destructor)) __exit()
{
    log_delete(__log);
    lock_delete(__lock);
    options_delete(options);
    inject_delect(__inject);
    object_delete(__config);
    curl_global_cleanup();
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
            if (__lock)
            {
                if (options->startup == positive)
                {
                    locking(__lock);
                    __config->log = __log;
                    __config->options = options;
                    inject_create(&__inject, options->conf);
                    if (__inject)
                    {
                        __config->inject = __inject;
                    }
                    return __config;
                }
                else if (options->startup == negative)
                {
                    lockexit(__lock);
                    exit(EXIT_SUCCESS);
                }
            }
        }
    }

    return NULL;
}

void config_delete(config_t *config)
{
    if (config && config->inject)
    {
        inject_delect(config->inject);
    }
}