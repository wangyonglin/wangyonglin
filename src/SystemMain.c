#include <stdio.h>
#include <stdlib.h>
#include <SystemString.h>
#include <SystemConfig.h>
#include <SystemDaemon.h>
#include <HTTPSServer.h>
#include <SnowFlake.h>
#include <StringUtils.h>
#include <MQTTManager.h>

#include <SystemSignal.h>
#include <ConfUtils.h>
#include <AllocateUtils.h>
// Better not to flood a public broker. Test against localhost.

SystemConfig_t *__SystemConfig = NULL;
MQTTManager_t *__MQTTManager = NULL;
HTTPSServer_t *__HTTPSServer =NULL;
int cleanup(void *arps)
{
    printf("\texit\r\n");
    MQTTManager_cleanup(__MQTTManager, __SystemConfig->AllocateUtils);
    HTTPSServer_cleanup(__HTTPSServer);
    SystemConfig_cleanup(__SystemConfig);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    SystemSignal_initializing(cleanup, NULL);
    SystemConfig_initializing(&__SystemConfig, argc, argv);
    MQTTManager_initializing(&__MQTTManager,__SystemConfig);
    HTTPSServer_initializing(&__HTTPSServer,__SystemConfig);
    HTTPSServer_router(__HTTPSServer);
    // MQTTManager_application(__MQTTManager);
    goto __exit;
__exit:
    cleanup(NULL);
}