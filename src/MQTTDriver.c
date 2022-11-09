#include <MQTTDriver.h>



#define MQTT_GROUP_CONF "MQTTALIYUN"
ok_t MQTTDriver_initializing(MQTTDriver_t **MQTTDriver, SystemAllocate_t *SystemAllocate, SystemConf_t *SystemConf, SystemLog_t *SystemLog)
{

    if (!SystemConf && !SystemAllocate && !SystemLog)
    {
        return ArgumentException;
    }
    if (((*MQTTDriver) = SystemAllocate_Create(SystemAllocate, sizeof(MQTTDriver_t))) == NULL)
    {
        return NullPointerException;
    }
    if (OK != (SystemConf_String(SystemConf, &(*MQTTDriver)->serverURI, MQTT_GROUP_CONF, "serverURI")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "serverURI");
        return ErrorException;
    }

    // if (OK != (SystemConf_String(SystemConf, &(*MQTTDriver)->clientID, MQTT_GROUP_CONF, "clientID")))
    // {
    //     SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "clientID");
    //     return ErrorException;
    // }
    if (OK != (SystemConf_String(SystemConf, &(*MQTTDriver)->productKey, MQTT_GROUP_CONF, "productKey")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "productKey");
        return ErrorException;
    }
    if (OK != (SystemConf_String(SystemConf, &(*MQTTDriver)->deviceName, MQTT_GROUP_CONF, "deviceName")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "deviceName");
        return ErrorException;
    }
    if (OK != (SystemConf_String(SystemConf, &(*MQTTDriver)->deviceSecret, MQTT_GROUP_CONF, "deviceSecret")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "deviceSecret");
        return ErrorException;
    }
    if (OK != (SystemConf_Number(SystemConf, &(*MQTTDriver)->minRetryInterval, MQTT_GROUP_CONF, "minRetryInterval")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "minRetryInterval");
        return ErrorException;
    }
    if (OK != (SystemConf_Number(SystemConf, &(*MQTTDriver)->maxRetryInterval, MQTT_GROUP_CONF, "maxRetryInterval")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "maxRetryInterval");
        return ErrorException;
    }
    if (OK != (SystemConf_Number(SystemConf, &(*MQTTDriver)->cleansession, MQTT_GROUP_CONF, "cleansession")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "cleansession");
        return ErrorException;
    }
    if (OK != (SystemConf_Number(SystemConf, &(*MQTTDriver)->keepAliveInterval, MQTT_GROUP_CONF, "keepAliveInterval")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "keepAliveInterval");
        return ErrorException;
    }
    if (OK != (SystemConf_Number(SystemConf, &(*MQTTDriver)->automaticReconnect, MQTT_GROUP_CONF, "automaticReconnect")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "automaticReconnect");
        return ErrorException;
    }
    if (OK != (SystemConf_Number(SystemConf, &(*MQTTDriver)->maxInflight, MQTT_GROUP_CONF, "maxInflight")))
    {
        SystemLog_error(SystemLog, "配置文件读取错误{%s}{%s}", MQTT_GROUP_CONF, "maxInflight");
        return ErrorException;
    }
    return OK;
}
