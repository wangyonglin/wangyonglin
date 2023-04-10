#include <wangyonglin/options.h>

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

options_t *options_create(options_t **options, int argc, char *argv[])
{

    if (((*options) = (options_t *)global_hooks.allocate(sizeof(options_t))))
    {
        (*options)->daemonize = negative;
        (*options)->startup = positive;

        while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
        {
            switch (opt)
            {
            case 'c':
                string_create(&(*options)->conf, optarg, strlen(optarg));
                break;
            case 'i':

                break;
            case 's':
                if (!strcmp(optarg, "start"))
                {
                    (*options)->startup = positive;
                }
                else if (!strcmp(optarg, "stop"))
                {
                    (*options)->startup = negative;
                }
                else if (!strcmp(optarg, "status"))
                {
                    (*options)->startup = invalid;
                }
                else
                {
                    fprintf(stderr, "starting params error[%s]", optarg);
                    return NULL;
                }
                break;
            case 'd':
                (*options)->daemonize = positive;
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

        if (!(*options)->conf.valuestring)
        {
            size_t maxsize = 0;
            maxsize += sizeof(PACKAGE_DIRECTERY_CONF);
            maxsize++;
            maxsize += sizeof(PACKAGE_NAME);
            maxsize += sizeof(".conf");
            maxsize++;
            char tmpString[maxsize];
            memset(&tmpString, 0x00, sizeof(tmpString));
            strcat(tmpString, PACKAGE_DIRECTERY_CONF);
            strcat(tmpString, "/");
            strcat(tmpString, PACKAGE_NAME);
            strcat(tmpString, ".conf");
            string_create(&(*options)->conf, tmpString, strlen(tmpString));
        }

        return (*options);
    }

    return NULL;
}

void options_delete(options_t *options)
{
    if (options)
    {
        string_delete(options->conf);
        global_hooks.deallocate(options);
    }
}