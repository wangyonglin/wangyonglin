#if !defined(__USER_CONFIG_H__)
#define __USER_CONFIG_H__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //for getopt, fork
#include <string.h> //for strcat
#include <signal.h>
#include <unix/log.h>
#include <unix/pid.h>
typedef struct
{
    char *conf_filename;
    int daemon;
    int httpd_option_timeout;
    char *pid_filename;
    log_t *log;
    pid_mk_t *pid;
} user_config_t;

user_config_t *user_config_crt(user_config_t **config, int argc, char *argv[]);
void user_config_del(user_config_t *config);
pid_t daemonize(void);
#endif