#include <SystemOptions.h>
#include <SystemString.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h> //getopt_long
#include <string.h>
#include <SystemError.h>
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

/*
config_options_t *config_options_allocate(config_options_t **options)
{
    if (allocate_object((void **)options, sizeof(config_options_t)) == NULL)
    {
        perror("config_options_allocate 申请内存结构体失败");
        return NULL;
    }
    {
        allocate_string(&((*options)->ini_filename), STRING_MALLOC_MAX);
        allocate_string(&((*options)->pid_filename), STRING_MALLOC_MAX);
        allocate_string(&((*options)->log_filename), STRING_MALLOC_MAX);
        return (*options);
    }

    return NULL;
}

void config_options_deallocate(config_options_t *options)
{
    deallocate_string(options->pid_filename);
    deallocate_string(options->ini_filename);
    deallocate_string(options->log_filename);
    deallocate_object(options);
}

ok_t config_options_initializing(config_options_t *options, int argc, char *argv[])
{
    if (options)
    {
        setting_string(options->ini_filename, strdup(ini_filename), strlen(ini_filename));
        setting_string(options->log_filename, strdup(log_filename), strlen(log_filename));
        setting_string(options->pid_filename, strdup(pid_filename), strlen(pid_filename));
        options->started = start;
        options->deamoned = FALSE;
        while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
        {
            switch (opt)
            {
            case 'c':
                err_printf(optarg);
                string_upa(&(options->ini_filename), strdup(optarg), strlen(optarg));
                break;
            case 'i':
                err_printf(optarg);
                string_upa(&(options->pid_filename), strdup(optarg), strlen(optarg));
                break;
            case 's':
                if (!strcmp(optarg, "start"))
                {
                    options->started = start;
                }
                else if (!strcmp(optarg, "stop"))
                {
                    options->started = stop;
                }
                else
                {
                    err_printf("starting params error[%s]", optarg);
                    return OK_ERROR;
                }
                break;
            case 'd':
                options->deamoned = TRUE;
                break;
            case 'v':
                err_printf("v");
                return OK_NONE;
                break;
            case '?': //未定义参数项
                printf("arg err:\r\n");
                printf("Try 'getopt_test -h' for more information.\r\n");
                return OK_NONE;
                break;
            default:
                printf("getopt_test: invalid option -- '%c'\r\n", opt);
                printf("Try 'getopt_test -h' for more information.\r\n");
                return OK_NONE;
                break;
            }
        }
        return OK_SUCCESS;
    }
    return OK_ERROR;
}
*/
const char *ini_filename = "/home/wangyonglin/github/wangyonglin/conf/tiger.conf";
const char *pid_filename = "/var/run/wangyonglin.pid";
const char *log_filename = "/home/wangyonglin/github/wangyonglin/logs/error.log";
#define STRING_MALLOC_MAX 512

char ini_filename_buffer[64] = {0};
char pid_filename_buffer[64] = {0};
char log_filename_buffer[64] = {0};
ok_t SystemOptions_initializing(SystemOptions_t **SystemOptions,AllocateUtils_t *AllocateUtils, int argc, char *argv[])
{
    if (!AllocateUtils)
    {
        return ArgumentException;
    }

    if (!((*SystemOptions) = AllocateUtils_pool(AllocateUtils, sizeof(SystemOptions_t))))
    {
        return NullPointerException;
    }
    strncpy(ini_filename_buffer, ini_filename, strlen(ini_filename));
    strncpy(pid_filename_buffer, pid_filename, strlen(pid_filename));
    strncpy(log_filename_buffer, log_filename, strlen(log_filename));
    while (-1 != (opt = getopt_long(argc, argv, short_options, long_options, &option_index)))
    {
        switch (opt)
        {
        case 'c':
            bzero(ini_filename_buffer, sizeof(ini_filename_buffer));
            strncpy(ini_filename_buffer, strdup(optarg), strlen(optarg));
            break;
        case 'i':
            bzero(pid_filename_buffer, sizeof(pid_filename_buffer));
            strncpy(pid_filename_buffer, strdup(optarg), strlen(optarg));
            break;
        case 's':
            if (!strcmp(optarg, "start"))
            {
                (*SystemOptions)->started = on_start;
            }
            else if (!strcmp(optarg, "stop"))
            {
                (*SystemOptions)->started = on_stop;
            }
            else
            {
                err_printf("starting params error[%s]", optarg);
                return ErrorException;
            }
            break;
        case 'd':
            (*SystemOptions)->deamoned = true;
            break;
        case 'v':
            err_printf("v");
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

    if (!(AllocateUtils_toString(&(*SystemOptions)->ini_filename, AllocateUtils, strdup(ini_filename_buffer), strlen(ini_filename_buffer))))
    {
        return ErrorException;
    }
    if (!(AllocateUtils_toString(&(*SystemOptions)->pid_filename, AllocateUtils, strdup(pid_filename_buffer), strlen(pid_filename_buffer))))
    {
        return ErrorException;
    }
    if (!(AllocateUtils_toString(&(*SystemOptions)->log_filename, AllocateUtils, strdup(log_filename_buffer), strlen(log_filename_buffer))))
    {
        return ErrorException;
    }

    return OK;
}

void SystemOptions_clean()
{
    bzero(ini_filename_buffer, sizeof(ini_filename_buffer));
    bzero(pid_filename_buffer, sizeof(pid_filename_buffer));
    bzero(log_filename_buffer, sizeof(log_filename_buffer));
}