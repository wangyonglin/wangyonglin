#include <wangyonglin/wangyonglin.h>
#include <curl/curl.h>
#include <Stringex.h>
#include <zlog/zlog.h>
#include <SnowFlake.h>
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
        event_loopbreak();
        break;
    }
    exit(-1);
}

void __attribute__((constructor)) init()
{
    if (ConfigCreate())
    {
        SnowFlakeInit(1, 1, 10);
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    return;
}
void __attribute__((destructor)) __exit()
{
    curl_global_cleanup();
    ConfigDelete();
    inject_delect(__inject);
}
Config_t *ConfigCreate()
{
    if (ObjectCreate((void **)&config, sizeof(Config_t)))
    {
        // 自定义信号处理函数
        signal(SIGHUP, sigintHandler);
        signal(SIGTERM, sigintHandler);
        signal(SIGINT, sigintHandler);
        signal(SIGQUIT, sigintHandler);

        if (!OptCreate(&config->options))
        {
            return NULL;
        }

        if (zlog_init(PACKAGE_CONF_ZLOG_FILENAME))
        {
            ObjectDelete(config);
            fprintf(stderr, "init failed %s not exist\n", PACKAGE_CONF_ZLOG_FILENAME);
            exit(EXIT_FAILURE);
            return NULL;
        }

        if (!((config->zlog) = zlog_get_category("zlog_rules")))
        {
            ObjectDelete(config);
            fprintf(stderr, "zlog_get_category [zlog_rules] failed\n");
            zlog_fini();
            exit(EXIT_FAILURE);
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
        if (OptInit(config->options, argc, argv))
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
    zlog_fini(); // 清理
    ObjectDelete(config->options);
    lock_delete(config->lock);
    if (config && config->inject)
    {
        inject_delect(config->inject);
    }
    object_delete(config);
}