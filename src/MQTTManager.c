
#include <MQTTManager.h>
#include <SystemTime.h>
#include <MQTTDriver.h>

// Better not to flood a public broker. Test against localhost.
#define TOPIC "/a1v3yiXDwzX/VEN_B7/user/system/post"
#define QOS 1
#define TIMEOUT 10000L
#define SAMPLE_PERIOD 1L // in ms，间隔1毫秒发一条消息

void connlost(void *context, char *cause)
{
    MQTTManager_t *__MQTTManager = (MQTTManager_t *)context;
    if (__MQTTManager->SystemLog && __MQTTManager)
    {
        SystemLog_error(__MQTTManager->SystemLog, "ERROR:Connection lost,Cause: %s,Reconnecting...", cause);
        __MQTTManager->connected = false;
    }
}
void onConnectCallCBack(void *context, char *cause)
{
    MQTTManager_t *__MQTTManager = (MQTTManager_t *)context;
    if (__MQTTManager->SystemLog && __MQTTManager)
    {
        SystemLog_error(__MQTTManager->SystemLog, "Successful onConnectCallCBack: %s", cause);
        __MQTTManager->connected = true;
    }
}

void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    printf("Disconnect failed\n");
}
void onDisconnect(void *context, MQTTAsync_successData *response)
{
    printf("Successful disconnection\n");
}

void onSendFailure(void *context, MQTTAsync_failureData *response)
{
    printf("Message send failed token %d error code %d\n", response->token, response->code);

#if 0 // disconnect  会导致回调函数失效，进而导致重连逻辑失效，要注意
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
 
    opts.onSuccess = onDisconnect;
    opts.onFailure = onDisconnectFailure;
    opts.context = client;
    int rc;
    if ((rc = MQTTAsync_disconnect(client, &opts)) == MQTTASYNC_SUCCESS)
    {
        printf("success to start disconnect , return code %d\n", rc);
    }
    else
    {
        printf("Failed to start disconnect, return code %d\n", rc);
    }
#endif
}
void onSend(void *context, MQTTAsync_successData *response)
{
    // This gets called when a message is acknowledged successfully.
}

void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    MQTTManager_t *__MQTTManager = (MQTTManager_t *)context;
    if (__MQTTManager->SystemLog && __MQTTManager)
    {
        SystemLog_error(__MQTTManager->SystemLog, "Connect failed, rc %s\n", response ? MQTTAsync_strerror(response->code) : "none");
        __MQTTManager->finished = true;
    }
}
void onConnect(void *context, MQTTAsync_successData *response)
{
    MQTTManager_t *__MQTTManager = (MQTTManager_t *)context;
    if (__MQTTManager->SystemLog && __MQTTManager)
    {
        SystemLog_error(__MQTTManager->SystemLog, "Successful connection");
        __MQTTManager->connected = true;
    }
}

int messageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *message)
{

    // printf("Message arrived\n");
    // printf("    topic: %s\n", topicName);
    // printf("    message: %.*s\n", message->payloadlen, (char *)message->payload);
    // MQTTAsync_freeMessage(&message);
    // MQTTAsync_free(topicName);
    return 1;
}

ok_t MQTTManager_initializing(MQTTManager_t **MQTTManager, SystemAllocate_t *SystemAllocate, SystemConf_t *SystemConf, SystemLog_t *SystemLog)
{
    if (!SystemAllocate && !SystemConf && !SystemLog)
    {
        return ArgumentException;
    }
    if (((*MQTTManager) = SystemAllocate_Create(SystemAllocate, sizeof(MQTTManager_t))) == NULL)
    {
        return NullPointerException;
    }
    (*MQTTManager)->SystemLog = SystemLog;

    char clientID[150] = {0};
    char username[65] = {0};
    char password[65] = {0};
    if (OK != MQTTDriver_initializing(&(*MQTTManager)->MQTTDriver, SystemAllocate, SystemConf, SystemLog))
    {
        SystemLog_error(SystemLog, "MQTTDriver_initializing filed");
        return ErrorException;
    }
    SystemLog_error(SystemLog, "serverURI{%s}", (*MQTTManager)->MQTTDriver->serverURI);
    SystemLog_error(SystemLog, "productKey{%s}", (*MQTTManager)->MQTTDriver->productKey);
    SystemLog_error(SystemLog, "deviceName{%s}", (*MQTTManager)->MQTTDriver->deviceName);
    SystemLog_error(SystemLog, "deviceSecret{%s}", (*MQTTManager)->MQTTDriver->deviceSecret);
    if (OK != (MQTTAliyun_Signature((*MQTTManager)->MQTTDriver->productKey, (*MQTTManager)->MQTTDriver->deviceName, (*MQTTManager)->MQTTDriver->deviceSecret, clientID, username, password)))
    {
        SystemLog_error(SystemLog, "MQTTAliyun_Signature filed");
        return ErrorException;
    }
    printf("\tclientId:%s\r\n", clientID);
    printf("\tusername:%s\r\n", username);
    printf("\tpassword:%s\r\n", password);

    MQTTAsync_connectOptions connectOptions = MQTTAsync_connectOptions_initializer;
    connectOptions.minRetryInterval = (*MQTTManager)->MQTTDriver->minRetryInterval;
    connectOptions.maxRetryInterval = (*MQTTManager)->MQTTDriver->maxRetryInterval;
    connectOptions.cleansession = (*MQTTManager)->MQTTDriver->cleansession;
    connectOptions.keepAliveInterval = (*MQTTManager)->MQTTDriver->keepAliveInterval;
    connectOptions.automaticReconnect = (*MQTTManager)->MQTTDriver->automaticReconnect;
    connectOptions.maxRetryInterval = (*MQTTManager)->MQTTDriver->maxRetryInterval;
    connectOptions.maxInflight = (*MQTTManager)->MQTTDriver->maxInflight;

    connectOptions.username = username;
    connectOptions.password = password;
    connectOptions.onSuccess = onConnect;
    connectOptions.onFailure = onConnectFailure;
    connectOptions.context = (*MQTTManager);


    int rc;
    if ((rc = MQTTAsync_create(&(*MQTTManager)->client, (*MQTTManager)->MQTTDriver->serverURI, clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
    {

        SystemLog_error(SystemLog, "[MQTT]  Failed to create client object, return code %d\n", rc);
        return rc;
    }
    // 设置连接丢失回调
    if ((rc = MQTTAsync_setCallbacks((*MQTTManager)->client, (*MQTTManager), connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
    {
        SystemLog_error(SystemLog, "Failed to set callback, return code %d\n", rc);
        return rc;
    }
    // 设置连接成功回调
    if ((rc = MQTTAsync_setConnected((*MQTTManager)->client, (*MQTTManager), onConnectCallCBack)) != MQTTASYNC_SUCCESS)
    {
        SystemLog_error(SystemLog, "[MQTT] Failed to set connected callback, return code %d\n", rc);
        return rc;
    }

    if ((rc = MQTTAsync_connect((*MQTTManager)->client, &connectOptions)) != MQTTASYNC_SUCCESS)
    {
        SystemLog_error(SystemLog, "[MQTT] Failed to start connect, return code %d\n", rc);
        return rc;
    }
    
    return OK;
}

void MQTTManager_cleanup(MQTTManager_t *MQTTManager)
{
    if (MQTTManager)
    {
        MQTTAsync_destroy(&(MQTTManager->client));
    }
}

int MQTTManager_application(MQTTManager_t *MQTTManager)
{
    int ret;
    MQTTAsync_message message = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions responseOptions = MQTTAsync_responseOptions_initializer;
    while (!MQTTManager->connected)
    {

        usleep(100000L);
    }

    while (!MQTTManager->finished)
    {
        if (1 == MQTTManager->connected)
        {
            usleep(300000L);
            int64_t t = SystemTime_milliseconds();

            char buf[256];
            int n = snprintf(buf, sizeof(buf), "%lld", (long long)t);
            printf("SEND:%s\n", buf);

            responseOptions.onSuccess = onSend;
            responseOptions.onFailure = onSendFailure;
            responseOptions.context = MQTTManager;

            message.payload = buf;
            message.payloadlen = n;
            message.qos = QOS;
            message.retained = 0;

            if ((ret = MQTTAsync_sendMessage(MQTTManager->client, TOPIC, &message, &responseOptions)) != MQTTASYNC_SUCCESS)
            {
                SystemLog_error(MQTTManager->SystemLog, "Failed to start sendMessage, return code %d\n", ret);
                MQTTManager->connected = false;
            }
        }
        else
        {
            printf("mqtt disconnected...\n");
            usleep(1000 * 1000);
        }

        usleep(SAMPLE_PERIOD * 1000);
    }
}
