#if !defined(DEVELOPER_CONFIG_H)
#define DEVELOPER_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <developer/public.h>
#include <developer/allocate.h>
#include <wangyonglin/conf.h>
#include <developer/args.h>
#include <developer/pidlock.h>
#include <developer/daemon.h>
#include <developer/log.h>
#include <developer/message.h>
#include <wangyonglin/string.h>


typedef struct
{
    allocate_t *allocate;
    args_t *args;
    log_t *log;
    message_id_t msg1;
    message_id_t msg2;
    message_id_t msg3;
    message_id_t msg4;
} config_t;

ok_t config_create(config_t **config, size_t allocate_max_size, int argc, char *argv[]);
void config_destroy(config_t *config);
ok_t config_verify(config_t *config);

#endif