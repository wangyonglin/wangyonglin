#include <wangyonglin/wangyonglin.h>
#include <curl/curl.h>
Config_t *config = NULL;
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
    ConfigCreate();

    curl_global_init(CURL_GLOBAL_DEFAULT);
}
void __attribute__((destructor)) __exit()
{
    ConfigDelete();
    inject_delect(__inject);

    curl_global_cleanup();
}
Config_t *ConfigCreate()
{
    if (object_create((void **)&config, sizeof(Config_t)))
    {

        if (!OptCreate(&config->options))
        {
            return NULL;
        }
        if (!log_create(&config->log))
        {
            return NULL;
        }
        if (!lock_create(&config->lock))
        {
            return NULL;
        }
    }
    return config;
}
Config_t *ConfigInit(int argc, char *argv[])
{
    if (config)
    {
        if (OptInit(&config->options, argc, argv))
        {
            if (config->options->daemonize == positive && config->options->startup == positive)
            {
                daemonize();
            }

            if (config->options->startup == positive)
            {
                locking(config->lock);

                inject_create(&__inject, config->options->ini);
                if (__inject)
                {
                    config->inject = __inject;
                }
                return config;
            }
            else if (config->options->startup == negative)
            {
                lockexit(config->lock);
                exit(EXIT_SUCCESS);
            }
        }
    }
    exit(EXIT_FAILURE);
}

void ConfigDelete()
{
    OptDelete(config->options);
    lock_delete(config->lock);
    log_delete(config->log);
    if (config && config->inject)
    {
        inject_delect(config->inject);
    }
    object_delete(config);
}