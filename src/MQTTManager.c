#include <MQTTManager.h>
// Better not to flood a public broker. Test against localhost.
#define TOPIC "/a1v3yiXDwzX/VEN_B7/user/system/post"
#define QOS 1
#define TIMEOUT 10000L
#define SAMPLE_PERIOD 1L // in ms，间隔1毫秒发一条消息

MQTTAsync_connectOptions connectOptions = MQTTAsync_connectOptions_initializer;
MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;
void connlost(void *context, char *cause)
{
    printf("\nERROR:Connection lost,Cause: %s,Reconnecting...\n", cause);
    if (context)
    {
        MQTTManager_connected(context, false);
    }
}
void onConnectCallCBack(void *context, char *cause)
{
    printf("Successful onConnectCallCBack\n");
    if (context)
    {
        MQTTManager_connected(context, true);
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
    // printf("Connect failed, rc %d [%s]\n", response ? response->code : 0, response->message);
    fprintf(stderr, "Connect failed, rc %s\n", response ? MQTTAsync_strerror(response->code) : "none");
    if (context)
    {
        MQTTManager_finished(context, 1);
    }
}
void onConnect(void *context, MQTTAsync_successData *response)
{
    printf("Successful connection\n");
    MQTTManager_t *manager = (MQTTManager_t *)context;
    if (manager)
    {
        MQTTManager_connected(manager, true);
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

int64_t getTime(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ((int64_t)ts.tv_sec * 1000) + ((int64_t)ts.tv_nsec / 1000000);
}

ok_t MQTTManager_initializing(MQTTManager_t **manager, MQTTAliyun_Config_t * MQTTAliyun_Config)
{
    if (((*manager) = (MQTTManager_t *)malloc(sizeof(MQTTManager_t))) == NULL)
    {
        printf("MQTTManager_Initating malloc failed");
        return NullPointerException;
    }
    memset((*manager), 0x00, sizeof(MQTTManager_t));

    MQTTManager_serverURI((*manager), MQTTAliyun_Config->serverURI);
    MQTTManager_clientId((*manager),  MQTTAliyun_Config->clientID);
    MQTTManager_connectOptions_initializer((*manager));
    MQTTManager_minRetryInterval((*manager), MQTTAliyun_Config->minRetryInterval);
    MQTTManager_maxRetryInterval((*manager), MQTTAliyun_Config->maxRetryInterval);
    MQTTManager_cleansession((*manager), MQTTAliyun_Config->cleansession);
    MQTTManager_keepAliveInterval((*manager), MQTTAliyun_Config->keepAliveInterval);
    MQTTManager_context((*manager), (*manager));
    MQTTManager_Username((*manager), MQTTAliyun_Config->username);
    MQTTManager_Passwrod((*manager), MQTTAliyun_Config->password);
    MQTTManager_onSuccess((*manager), onConnect);
    MQTTManager_onFailure((*manager), onConnectFailure);
    MQTTManager_automaticReconnect(*manager, MQTTAliyun_Config->automaticReconnect);
    MQTTManager_maxInflight(*manager, MQTTAliyun_Config->maxInflight);
    return OK;
}

void MQTTManager_cleanup(MQTTManager_t *manager)
{
    if (manager)
    {
        MQTTAsync_destroy(&(manager->client));
        free(manager);
    }
}

void MQTTManager_connected(MQTTManager_t *manager, bool connected)
{
    if (manager)
    {
        manager->connected = connected;
    }
}
void MQTTManager_finished(MQTTManager_t *manager, int finished)
{
    if (manager)
    {
        manager->finished = finished;
    }
}
void MQTTManager_serverURI(MQTTManager_t *manager, const char *serverURI)
{
    if (manager)
    {
        manager->serverURI = strdup(serverURI);
    }
}
void MQTTManager_clientId(MQTTManager_t *manager, const char *clientId)
{
    if (manager)
    {
        manager->clientId = strdup(clientId);
    }
}

void MQTTManager_automaticReconnect(MQTTManager_t *manager, int automaticReconnect)
{
    if (manager)
    {
        manager->connectOptions.automaticReconnect = automaticReconnect;
    }
}

void MQTTManager_onSuccess(MQTTManager_t *manager, MQTTAsync_onSuccess *onSuccess)
{
    if (manager)
    {
        manager->connectOptions.onSuccess = onSuccess;
    }
}
void MQTTManager_onFailure(MQTTManager_t *manager, MQTTAsync_onFailure *onFailure)
{
    if (manager)
    {
        manager->connectOptions.onFailure = onFailure;
    }
}
void MQTTManager_Passwrod(MQTTManager_t *manager, const char *password)
{
    if (manager)
    {
        manager->connectOptions.password = password;
    }
}
void MQTTManager_Username(MQTTManager_t *manager, const char *username)
{
    if (manager)
    {
        manager->connectOptions.username = username;
    }
}

void MQTTManager_maxInflight(MQTTManager_t *manager, int maxInflight)
{
    if (manager)
    {
        manager->connectOptions.maxInflight = maxInflight;
    }
}
void MQTTManager_context(MQTTManager_t *manager, void *context)
{
    if (manager && context)
    {
        manager->connectOptions.context = context;
    }
}

void MQTTManager_keepAliveInterval(MQTTManager_t *manager, int keepAliveInterval)
{
    if (manager)
    {
        manager->connectOptions.keepAliveInterval = keepAliveInterval;
    }
}

void MQTTManager_cleansession(MQTTManager_t *manager, int cleansession)
{
    if (manager)
    {
        manager->connectOptions.cleansession = cleansession;
    }
}

void MQTTManager_maxRetryInterval(MQTTManager_t *manager, int maxRetryInterval)
{
    if (manager)
    {
        manager->connectOptions.maxRetryInterval = maxRetryInterval;
    }
}
void MQTTManager_minRetryInterval(MQTTManager_t *manager, int minRetryInterval)
{
    if (manager)
    {
        manager->connectOptions.minRetryInterval = minRetryInterval;
    }
}
void MQTTManager_connectOptions_initializer(MQTTManager_t *manager)
{
    if (manager)
    {
        manager->connectOptions = (MQTTAsync_connectOptions)MQTTAsync_connectOptions_initializer;
    }
}

int MQTTManager_application(MQTTManager_t *manager)
{
    int rc;
    if ((rc = MQTTAsync_create(&manager->client, manager->serverURI, manager->clientId, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to create client object, return code %d\n", rc);
        return rc;
    }
    // 设置连接丢失回调
    if ((rc = MQTTAsync_setCallbacks(manager->client, manager, connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to set callback, return code %d\n", rc);
        return rc;
    }
    // 设置连接成功回调
    if ((rc = MQTTAsync_setConnected(manager->client, manager, onConnectCallCBack)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to set connected callback, return code %d\n", rc);
        return rc;
    }

    if ((rc = MQTTAsync_connect(manager->client, &(manager->connectOptions))) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
        return rc;
    }
    while (!manager->connected)
    {

        usleep(100000L);
    }

    while (!manager->finished)
    {
        if (1 == manager->connected)
        {
            usleep(300000L);
            int64_t t = getTime();

            char buf[256];
            int n = snprintf(buf, sizeof(buf), "%lld", (long long)t);
            printf("SEND:%s\n", buf);

            pub_opts.onSuccess = onSend;
            pub_opts.onFailure = onSendFailure;
            pub_opts.context = manager;

            pubmsg.payload = buf;
            pubmsg.payloadlen = n;
            pubmsg.qos = QOS;
            pubmsg.retained = 0;

            if ((rc = MQTTAsync_sendMessage(manager->client, TOPIC, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS)
            {
                printf("Failed to start sendMessage, return code %d\n", rc);
                manager->connected = false;
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