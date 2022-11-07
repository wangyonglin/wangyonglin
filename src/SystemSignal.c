#include <SystemSignal.h>

SystemSignal_t systemSignal;
void cfinish(int sig)
{
    signal(SIGINT, NULL);
    systemSignal.exitCallback(systemSignal.exitCallbackParameters);
}
SystemSignal_t *SystemSignal_initializing(SystemSignal_exitCallback_t exitCallback, void *exitCallbackParameters)
{
    signal(SIGINT, cfinish);
    signal(SIGTERM, cfinish);
    if (exitCallback)
    {
        systemSignal.exitCallback = exitCallback;
        systemSignal.exitCallbackParameters= exitCallbackParameters;
    }
    return &systemSignal;
}
