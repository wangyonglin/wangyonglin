#include <stdio.h>
#include <stdlib.h>
#include <SystemString.h>
#include <SystemConfig.h>
#include <SystemDaemon.h>
#include <HTTPSServer.h>
#include <SystemPool.h>
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

SystemConfig_t *SystemConfig = NULL;
httpd_config_t *httpd;
MQTTAliyun_Config_t *MQTTAliyun_Config = NULL;
MQTTManager_t *MQTTManager = NULL;

int cleanup(void *arps);
int main(int argc, char *argv[])
{
    ok_t ret;
    SystemSignal_initializing(cleanup, NULL);
    SystemConfig_initializing(&SystemConfig, argc, argv);

    //  httpd_initializing(&httpd, "0.0.0.0", 80);
    //  httpd_router(httpd, NULL);

    MQTTAliyun_Config_initializing(&MQTTAliyun_Config, SystemConfig);

    STACK_OF(CONF_VALUE) *sect = NULL;
    CONF_VALUE *cnf;
    sect = NCONF_get_section(SystemConfig->SystemConf->conf, "MQTTAliyun");
    for (int i = 0; i < sk_CONF_VALUE_num(sect); i++)
    {
        cnf = sk_CONF_VALUE_value(sect, i);
        SystemLog_error(SystemConfig->SystemLog, "name:%s value:%s", cnf->name, cnf->value);
    }

    if ((ret = MQTTAliyun_Config_Signature(MQTTAliyun_Config) != OK))
    {
        printf("MQTTAliyun_Signature -%0x4x\n", -ret);
        return ErrorException;
    }
    printf("\tclientId:%s\r\n", MQTTAliyun_Config->clientID);
    printf("\tusername:%s\r\n", MQTTAliyun_Config->username);
    printf("\tpassword:%s\r\n", MQTTAliyun_Config->password);

    MQTTManager_initializing(&MQTTManager, MQTTAliyun_Config);
    MQTTManager_application(MQTTManager);

    return EXIT_FAILURE;
}
int cleanup(void *arps)
{
    printf("\texit\r\n");
    MQTTManager_cleanup(MQTTManager);
    MQTTAliyun_Config_Cleanup(MQTTAliyun_Config);
    SystemConfig_cleanup(SystemConfig);
    exit(EXIT_SUCCESS);
}