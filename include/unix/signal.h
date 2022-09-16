#if !defined(__UNIX_SIGNAL_H__)
#define __UNIX_SIGNAL_H__

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
typedef void (*exit_handle_t)(void);
typedef void (*error_handle_t)(int sig);

void unix_signal_error(error_handle_t err);
void unix_signal_exit(exit_handle_t ext);
#endif