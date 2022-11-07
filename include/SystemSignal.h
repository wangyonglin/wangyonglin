#if !defined(INCLUDE_SYSTEMSIGNAL_H)
#define INCLUDE_SYSTEMSIGNAL_H
#include <stdio.h>
#include <memory.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
typedef  int (*SystemSignal_exitCallback_t)(void * vParameters);
typedef struct
{
    SystemSignal_exitCallback_t exitCallback;
    void * exitCallbackParameters;

} SystemSignal_t;

SystemSignal_t *SystemSignal_initializing(SystemSignal_exitCallback_t exitCallback, void *exitCallbackParameters);
#endif