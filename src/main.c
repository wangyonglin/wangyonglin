#include <stdio.h>
#include <stdlib.h>
#include <SystemString.h>
#include <SystemConfig.h>
#include <SystemDaemon.h>
#include <HTTPSServer.h>
#include <SnowFlake.h>
#include <StringUtils.h>
#include <MQTTManager.h>
#include <MQTTAliyunConfig.h>
#include <SystemSignal.h>

// Better not to flood a public broker. Test against localhost.

/*
   STACK_OF(CONF_VALUE) *sect = NULL;
   CONF_VALUE *cnf;
   sect = NCONF_get_section(config->conf->conf, "HTTPS");
   for (int i = 0; i < sk_CONF_VALUE_num(sect); i++)
   {
       cnf = sk_CONF_VALUE_value(sect, i);
       config_log_error(config->log, "name:%s value:%s", cnf->name, cnf->value);
   }
   */

SystemConfig_t *__SystemConfig = NULL;
// httpd_config_t *httpd;
// MQTTAliyun_Config_t *MQTTAliyun_Config = NULL;
MQTTManager_t *__MQTTManager = NULL;

int cleanup(void *arps);

int main(int argc, char *argv[])
{
    // ok_t ret;
    SystemSignal_initializing(cleanup, NULL);
    SystemConfig_initializing(&__SystemConfig, argc, argv);

    //  httpd_initializing(&httpd, "0.0.0.0", 80);
    //  httpd_router(httpd, NULL);

    MQTTManager_initializing(&__MQTTManager, __SystemConfig->SystemAllocate, __SystemConfig->SystemConf, __SystemConfig->SystemLog);
    MQTTManager_application(__MQTTManager);
    // SystemConfig_cleanup(SystemConfig);
__exit:
    cleanup(NULL);
}
int cleanup(void *arps)
{
    printf("\texit\r\n");
    MQTTManager_cleanup(__MQTTManager);
    SystemConfig_cleanup(__SystemConfig);
    exit(EXIT_SUCCESS);
}