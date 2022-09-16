#include <user_config.h>
#include <unix/string.h>
#include <getopt.h>
#include <fcntl.h>
#include "config.h"
#include <unix/pid.h>
#include <unix/signal.h>

const char *pid_filenames = "/var/run/wangyonglin.pid";

static struct option long_options[] = {
    {"daemon", no_argument, NULL, 'd'},
    {"conf", required_argument, NULL, 'c'},
    {"started", required_argument, NULL, 's'},
    {"version", no_argument, NULL, 'v'},
    {"help", no_argument, NULL, 'h'},
    {0, 0, 0, 0}};


user_config_t *user_config_crt(user_config_t **config, int argc, char *argv[])
{

    if ((*config) = (user_config_t *)global_hooks.allocate(sizeof(user_config_t)))
    {
      
        (*config)->daemon = 0;
        int option_index, c;
        while ((c = getopt_long(argc, argv, "c:s:vhd", long_options, &option_index)) != -1)
        {
            switch (c)
            {
            case 'h':
                fprintf(stdout, "\t %s \r\n", VERSION);
                global_hooks.deallocate((*config));
                exit(EXIT_SUCCESS);
            case 'v':
                fprintf(stdout, "%s\r\n", VERSION);
                global_hooks.deallocate((*config));
                exit(EXIT_SUCCESS);
            case 'd':
                printf("\t daemon \r\n");
                (*config)->daemon = 1;
                break;
            case 's':
                if (strcmp("stop", optarg) == 0)
                {
                    kill(getpid(), SIGKILL);
                }
                break;
            case 'c':

                (*config)->conf_filename = strdup(optarg);
                break;
            case 'P':

                break;
            case 'r':
                // audio_cfg.rate = strtol(optarg, NULL, 0);
                break;
            case '?':
                break;
            default:
                printf("?? getopt returned character code 0%o ??\n", c);
                global_hooks.deallocate((*config));
                exit(EXIT_SUCCESS);
            }
        }

        string_crt(&((*config)->pid_filename), strdup(pid_filenames), strlen(pid_filenames));
        //配置日志功能

        if (log_crt(&(*config)->log, "/home/wangyonglin/github/wangyonglin/logs/error.log", (*config)->daemon))
        {
            pid_crt(&((*config)->pid), (*config)->log, (*config)->pid_filename);
            return (*config);
        }
    }
    return NULL;
}

void user_config_del(user_config_t *config)
{
    if (config)
    {
        pid_del(config->pid, config->log);
        //删除日志功能
        log_del(config->log);
        string_del(config->conf_filename);
        global_hooks.deallocate(config);
    }
}

pid_t daemonize(void)
{ // come from /redis/server.c/daemonize()
    pid_t pid;
    int fd;
    if (fork() != 0)
        exit(0);    /* parent exits */
    pid = setsid(); /* create a new session */
    chdir("/");
    /* Every output goes to /dev/null. If Redis is daemonized but
     * the 'logfile' is set to 'stdout' in the configuration file
     * it will not log at all. */
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO)
            close(fd);
    }
    return pid;
}
