#include <MainConfig.h>
#include <Stringex.h>
#include <zlog/zlog.h>
#include <curl/curl.h>

MainConfig *mainConfig = NULL;
zlog_category_t *log = NULL;
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
    // printf("\t init ...\r\n");
    signal(SIGHUP, sigintHandler);
    signal(SIGTERM, sigintHandler);
    signal(SIGINT, sigintHandler);
    signal(SIGQUIT, sigintHandler);
    SnowFlakeInit(1, 1, 10);
    curl_global_init(CURL_GLOBAL_DEFAULT);
    return;
}
void __attribute__((destructor)) __exit()
{
    // printf("\t exit ...\r\n");
    curl_global_cleanup();
    ConfigDelete();
}

Boolean MainConfigCreate(MainConfig **mainConfig, int argc, char *argv[])
{
    Boolean err = Boolean_false;
    if (ObjectCreate((void **)mainConfig, sizeof(MainConfig)))
    {

        if (!MainOptionCreate(&(*mainConfig)->mainOption))
        {
            fprintf(stderr, "MainOptionCreate  failed\n");
            return err;
        }
        if (!MainOptionInit((*mainConfig)->mainOption, argc, argv))
        {
            fprintf(stderr, "MainOptionInit  failed\n");
            return err;
        }
        if ((*mainConfig)->mainOption->daemonize == Boolean_true)
        {
            daemonize();
        }
        if (!InjectObjectCreate(&(*mainConfig)->injectObject, (*mainConfig)->mainOption->confFilename))
        {
            fprintf(stderr, "MemoryInjectCreate  failed\n");
            return err;
        }
        // 注册配置文件的display_errors 值到MainConfig对象里
        InjectCommand injectCommand[] = {
            INJECT_COMMAND_BOOLEAN("display_errors", offsetof(MainConfig, display_errors)),
            INJECT_COMMAND_STRING("zlog_info", offsetof(MainConfig, zlog_info)),
            INJECT_COMMAND_STRING("zlog_error", offsetof(MainConfig, zlog_error))};
        InjectCommandInit((*mainConfig)->injectObject, injectCommand, (*mainConfig), "global");
        // 初始化 zlog_init 字符串值
        char zlog_init_values[512] = {0};
        strcat(zlog_init_values, "[global]\n");
        strcat(zlog_init_values, "strict init = true\n");
        strcat(zlog_init_values, "buffer min = 1024\n");
        strcat(zlog_init_values, "buffer max = 2048\n");
        strcat(zlog_init_values, "rotate lock file = /tmp/zlog.lock\n");
        strcat(zlog_init_values, "file perms = 600\n");
        strcat(zlog_init_values, "[formats]\n");
        strcat(zlog_init_values, "simple	= \"%d.%-8.8us %-5V [%-8.8p.%-8.8t %F %L] %m%n\"\n");
        strcat(zlog_init_values, "normal = \"%d [%V] %m%n\"\n");
        strcat(zlog_init_values, "[rules]\n");
        if ((*mainConfig)->display_errors == Boolean_true && (*mainConfig)->mainOption->daemonize != Boolean_true)
        {
            strcat(zlog_init_values, "zlog_rules.*		>stderr; normal\n");
        }
        if (Stringex_IsString((*mainConfig)->zlog_info) && Stringex_IsEmpty((*mainConfig)->zlog_info))
        {
            int len = (*mainConfig)->zlog_info.valuelength + 128;
            char tmp[len];
            memset(tmp, 0x00, len);
            sprintf(tmp, "zlog_rules.INFO    | /usr/local/wangyonglin/sbin/cronolog %s;normal\n", (*mainConfig)->zlog_info.valuestring);
            strcat(zlog_init_values, tmp);
        }
        if (Stringex_IsString((*mainConfig)->zlog_error) && Stringex_IsEmpty((*mainConfig)->zlog_error))
        {
            int len = (*mainConfig)->zlog_error.valuelength + 128;
            char tmp[len];
            memset(tmp, 0x00, len);
            sprintf(tmp, "zlog_rules.!INFO    | /usr/local/wangyonglin/sbin/cronolog %s;simple\n", (*mainConfig)->zlog_info.valuestring);
            strcat(zlog_init_values, tmp);
        }

        if (zlog_init_from_string(zlog_init_values))
        {
            fprintf(stderr, "zlog_init failed \r\n%s\r\n", zlog_init_values);
            return err;
        }

        if (!(((*mainConfig)->log) = zlog_get_category("zlog_rules")))
        {
            fprintf(stderr, "zlog_get_category [zlog_rules] failed\n");
            zlog_fini();
            return err;
        }
        InjectObjectLogger((*mainConfig)->injectObject, (*mainConfig)->log);
        log=(*mainConfig)->log;
        if (!MainLockCreate(&(*mainConfig)->mainLock))
        {

            fprintf(stderr, "zlog_get_category [zlog_rules] failed\n");
            zlog_fini();
            return err;
        }

        if ((*mainConfig)->mainOption->enabled == Boolean_true)
        {

            if (!MainLockInit((*mainConfig)->mainLock, (*mainConfig)->log))
            {
                return err;
            }
        }
        else if ((*mainConfig)->mainOption->enabled == Boolean_false)
        {
            MainLockExit((*mainConfig)->mainLock);
            exit(EXIT_SUCCESS);
        }
    }
    return Boolean_true;
}

void ConfigDelete()
{
    zlog_fini(); // 清理
    if (mainConfig)
    {
        MainLockDelete(mainConfig->mainLock);
        MainOptionDelete(mainConfig->mainOption);
        StringexDelete(mainConfig->zlog_error);
        StringexDelete(mainConfig->zlog_info);
        ObjectDelete(mainConfig);
    }
}