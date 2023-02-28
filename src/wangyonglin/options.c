#include <wangyonglin/options.h>
#include <wangyonglin/daemonize.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/regedit.h>
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

options_t *options_create(struct _pool_t *pool, int argc, char *argv[])
{
    struct _options_t *options;
    if (!object_create(pool, &options, sizeof(struct _options_t)))
    {
        return NULL;
    }
    options->daemonize = negative;
    options->startup = positive;
    while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
    {
        switch (opt)
        {
        case 'c':
            string_create(pool, &options->cfname, optarg, strlen(optarg));
            break;
        case 'i':

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
    if (options->daemonize == positive && options->startup == positive)
    {
        daemonize();
    }
    if (!options->cfname)
    {
        size_t cfnamesize = 0;
        cfnamesize += strlen(PACKAGE_DIRECTERY_PREFIX);
        cfnamesize += strlen("/conf/");
        cfnamesize += strlen(PACKAGE_NAME);
        cfnamesize += strlen(".conf");
        cfnamesize += 1;
        options->cfname = allocate(pool, cfnamesize);
        strcat(options->cfname, PACKAGE_DIRECTERY_PREFIX);
        strcat(options->cfname, "/conf/");
        strcat(options->cfname, PACKAGE_NAME);
        strcat(options->cfname, ".conf");
    }
    return options;
}
