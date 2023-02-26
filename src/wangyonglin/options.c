#include <wangyonglin/options.h>
#include <wangyonglin/daemonize.h>
#include <wangyonglin/conf.h>
#define STRINTG_BUFFER_MAX 64

int opt;
const char *short_options = "s:c:i:dv";
int option_index = 0;
struct option long_options[] = {
    {"daemon", no_argument, 0, 'd'},
    {"config", optional_argument, 0, 'c'},
    {"start", required_argument, 0, 's'},
    {"pid", optional_argument, 0, 'i'},
    {"version", no_argument, 0, 'v'},
    {0, 0, 0, 0}};

#define STRING_MALLOC_MAX 512

options_t *options_create(struct _app_t *app, int argc, char *argv[])
{

    if (!object_create(app->pool, &app->options, sizeof(struct _options_t)))
    {
        return NULL;
    }
    app->options->daemonize = negative;
    app->options->startup = positive;
    const char conffilename[128] = {0};
    strcat(conffilename, PACKAGE_DIRECTERY_PREFIX);
    strcat(conffilename,"/conf/wangyonglin.conf");
    while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
    {
        switch (opt)
        {
        case 'c':
            string_create(app->pool, &app->options->ini_file, optarg, strlen(optarg));
            break;
        case 'i':

            break;
        case 's':
            if (!strcmp(optarg, "start"))
            {
                app->options->startup = positive;
            }
            else if (!strcmp(optarg, "stop"))
            {
                app->options->startup = negative;
            }
            else if (!strcmp(optarg, "status"))
            {
                app->options->startup = invalid;
            }
            else
            {
                fprintf(stderr, "starting params error[%s]", optarg);
                return NULL;
            }
            break;
        case 'd':
            app->options->daemonize = positive;
            break;
        case 'v':
            fprintf(stderr, "v");
            return NoneException;
            break;
        case '?': // 未定义参数项
            printf("arg err:\r\n");
            printf("Try 'getopt_test -h' for more information.\r\n");

            return NULL;
            break;
        default:
            printf("getopt_test: invalid option -- '%c'\r\n", opt);
            printf("Try 'getopt_test -h' for more information.\r\n");

            return NULL;
            break;
        }
    }
    if (app->options->daemonize == positive && app->options->startup == positive)
    {
        daemonize();
    }
    if (!app->options->ini_file)
    {
        string_create(app->pool, &app->options->ini_file, conffilename, strlen(conffilename));
    }

    struct _command commands[] = {
        {"error_log", NULL, STRING, offsetof(struct _options_t, error_log)},
        {"lockfile", NULL, STRING, offsetof(struct _options_t, lockfile)},
        {"log_errors", NULL, BOOLEAN, offsetof(struct _options_t, log_errors)},
        null_command};
    command_init(app, app->options, commands, NULL);
    return app->options;
}
