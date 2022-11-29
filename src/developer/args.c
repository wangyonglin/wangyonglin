#include <developer/args.h>
#include <developer/allocate.h>
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

const char *ini_filename = "/home/wangyonglin/github/wangyonglin/conf/tiger.conf";

#define STRING_MALLOC_MAX 512

ok_t args_initializing(args_t **args, allocate_t *allocate, int argc, char *argv[])
{
    if (!allocate)
    {
        return ArgumentException;
    }

    if (object_create(allocate, (void **)args, sizeof(args_t) != Ok))
    {
        return ErrorException;
    }
    (*args)->daemoned = disabled;
    (*args)->started = onstart;
    while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
    {
        switch (opt)
        {
        case 'c':
            string_create(allocate, &(*args)->ini_filename, strdup(optarg), strlen(optarg));
            break;
        case 'i':

            break;
        case 's':
            if (!strcmp(optarg, "start"))
            {
                (*args)->started = onstart;
            }
            else if (!strcmp(optarg, "stop"))
            {
                (*args)->started = onstop;
            }
            else if (!strcmp(optarg, "status"))
            {
                (*args)->started = onstatus;
            }
            else
            {
                fprintf(stderr, "starting params error[%s]", optarg);
                return ErrorException;
            }
            break;
        case 'd':
            (*args)->daemoned = enabled;
            break;
        case 'v':
            fprintf(stderr, "v");
            return NoneException;
            break;
        case '?': // 未定义参数项
            printf("arg err:\r\n");
            printf("Try 'getopt_test -h' for more information.\r\n");
            return NoneException;
            break;
        default:
            printf("getopt_test: invalid option -- '%c'\r\n", opt);
            printf("Try 'getopt_test -h' for more information.\r\n");
            return NoneException;
            break;
        }
    }
    if (!(*args)->ini_filename)
    {
        string_create(allocate, &(*args)->ini_filename, strdup(ini_filename), strlen(ini_filename));
    }
    return Ok;
}
