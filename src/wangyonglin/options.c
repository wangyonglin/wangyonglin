#include <wangyonglin/options.h>

#define STRINTG_BUFFER_MAX 64

int opt;
const char *short_options = "s:c:l:i:dv";
int option_index = 0;
struct option long_options[] = {
    {"daemon", no_argument, 0, 'd'},
    {"config", optional_argument, 0, 'c'},
    {"log", optional_argument, 0, 'l'},
    {"start", required_argument, 0, 's'},
    {"pid", optional_argument, 0, 'i'},
    {"version", no_argument, 0, 'v'},
    {0, 0, 0, 0}};

#define STRING_MALLOC_MAX 512

options_t *OptCreate(options_t **options)
{
    if (((*options) = (options_t *)global_hooks.allocate(sizeof(options_t))))
    {
        (*options)->daemonize = negative;
        (*options)->startup = positive;
        (*options)->ini = Stringex_NULL;
        StringexResetting(&(*options)->ini, PACKAGE_CONF_MAIN_FILENAME, sizeof(PACKAGE_CONF_MAIN_FILENAME));
        StringexCreate(&(*options)->zlog, PACKAGE_CONF_ZLOG_FILENAME, sizeof(PACKAGE_CONF_ZLOG_FILENAME));
        return (*options);
    }
    return (*options) = NULL;
}
void OptDelete(options_t *options)
{
    global_hooks.deallocate(options);
}
options_t *OptInit(options_t *options, int argc, char *argv[])
{

    if (options)
    {

        while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
        {
            switch (opt)
            {
            case 'c':
                StringexResetting(&options->ini, optarg,strlen(optarg));
                break;
            case 'l':
                  StringexResetting(&options->zlog, optarg,strlen(optarg));
                break;
            case 's':
                if (!strcmp(optarg, "start"))
                {
                    options->startup = positive;
                }
                else if (!strcmp(optarg, "stop"))
                {
                    options->startup = negative;
                }
                else if (!strcmp(optarg, "status"))
                {
                    options->startup = invalid;
                }
                else
                {
                    fprintf(stderr, "starting params error[%s]", optarg);
                    return NULL;
                }
                break;
            case 'd':
                options->daemonize = positive;
                break;
            case 'v':
                return NoneException;
                break;
            case '?': // 未定义参数项
                return NULL;
                break;
            default:
                return NULL;
                break;
            }
        }

        return options;
    }

    return NULL;
}
