
#include <MQTTManager.h>
#include <SystemTime.h>
#include <ConfUtils.h>

ConfUtils_command_t __MQTTManager_commands[] = {
    {"serverURI", string, offsetof(MQTTManager_t, serverURI)},
    {"productKey", string, offsetof(MQTTManager_t, productKey)},
    {"deviceName", string, offsetof(MQTTManager_t, deviceName)},
    {"deviceSecret", string, offsetof(MQTTManager_t, deviceSecret)},
    {"minRetryInterval", integer, offsetof(MQTTManager_t, minRetryInterval)},
    {"maxRetryInterval", integer, offsetof(MQTTManager_t, maxRetryInterval)},
    {"cleansession", integer, offsetof(MQTTManager_t, cleansession)},
    {"automaticReconnect", integer, offsetof(MQTTManager_t, automaticReconnect)},
    {"keepAliveInterval", integer, offsetof(MQTTManager_t, keepAliveInterval)},
    {"maxInflight", integer, offsetof(MQTTManager_t, maxInflight)},
    {"daemon", boolean, offsetof(MQTTManager_t, daemon)}};

int MQTTManager_commands_size=sizeof(__MQTTManager_commands) / sizeof(__MQTTManager_commands[0]);

// Better not to flood a public broker. Test against localhost.
#define TOPIC "/a1v3yiXDwzX/VEN_B7/user/system/post"
#define QOS 1
#define TIMEOUT 10000L
#define SAMPLE_PERIOD 1L // in ms，间隔1毫秒发一条消息
char clientID[150] = {0};
char username[65] = {0};
char password[65] = {0};

volatile int finished = 0;
volatile int connected = 0;

void connlost(void *context, char *cause)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
    int rc;

    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);

    printf("Reconnecting\n");
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
        finished = 1;
    }
}

void onDisconnectFailure(void *context, MQTTAsync_failureData *response)
{
    printf("Disconnect failed\n");
    finished = 1;
}

void onDisconnect(void *context, MQTTAsync_successData *response)
{
    printf("Successful disconnection\n");
    finished = 1;
}

void onSendFailure(void *context, MQTTAsync_failureData *response)
{
    MQTTAsync client = (MQTTAsync)context;
    MQTTAsync_disconnectOptions opts = MQTTAsync_disconnectOptions_initializer;
    int rc;

    printf("Message send failed token %d error code %d\n", response->token, response->code);
    opts.onSuccess = onDisconnect;
    opts.onFailure = onDisconnectFailure;
    opts.context = client;
    if ((rc = MQTTAsync_disconnect(client, &opts)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start disconnect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
}

void onSend(void *context, MQTTAsync_successData *response)
{
    // This gets called when a message is acknowledged successfully.
}

void onConnectFailure(void *context, MQTTAsync_failureData *response)
{
    printf("Connect failed, rc %d\n", response ? response->code : 0);
    finished = 1;
}

void onConnect(void *context, MQTTAsync_successData *response)
{
    printf("Successful connection\n");
    connected = 1;
}

int messageArrived(void *context, char *topicName, int topicLen, MQTTAsync_message *m)
{
    /* not expecting any messages */
    return 1;
}

int64_t getTime(void)
{
#if defined(_WIN32)
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    return ((((int64_t)ft.dwHighDateTime) << 8) + ft.dwLowDateTime) / 10000;
#else
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ((int64_t)ts.tv_sec * 1000) + ((int64_t)ts.tv_nsec / 1000000);
#endif
}

ok_t MQTTManager_initializing(MQTTManager_t **MQTTManager,SystemConfig_t *SystemConfig)
{
    if (!SystemConfig->AllocateUtils && !SystemConfig->ConfUtils && !SystemConfig->SystemLog)
    {
        return ArgumentException;
    }
    if (((*MQTTManager) = AllocateUtils_pool(SystemConfig->AllocateUtils, sizeof(MQTTManager_t))) == NULL)
    {
        return NullPointerException;
    }
    memset((*MQTTManager), 0x00, sizeof(MQTTManager_t));

    ConfUtils_final(SystemConfig->ConfUtils, (void **)MQTTManager, sizeof(MQTTManager_t), "MQTTALIYUN", __MQTTManager_commands, MQTTManager_commands_size);

    SystemLog_info(SystemConfig->SystemLog, "serverURI   {%s}", (*MQTTManager)->serverURI);
    SystemLog_info(SystemConfig->SystemLog, "productKey  {%s}", (*MQTTManager)->productKey);
    SystemLog_info(SystemConfig->SystemLog, "deviceName  {%s}", (*MQTTManager)->deviceName);
    SystemLog_info(SystemConfig->SystemLog, "deviceSecret    {%s}", (*MQTTManager)->deviceSecret);
    SystemLog_info(SystemConfig->SystemLog, "minRetryInterval    {%d}", (*MQTTManager)->minRetryInterval);
    SystemLog_info(SystemConfig->SystemLog, "maxRetryInterval    {%d}", (*MQTTManager)->maxRetryInterval);
    SystemLog_info(SystemConfig->SystemLog, "cleansession    {%d}", (*MQTTManager)->cleansession);
    SystemLog_info(SystemConfig->SystemLog, "automaticReconnect  {%d}", (*MQTTManager)->automaticReconnect);
    SystemLog_info(SystemConfig->SystemLog, "keepAliveInterval  {%d}", (*MQTTManager)->keepAliveInterval);
    SystemLog_info(SystemConfig->SystemLog, "maxInflight {%d}", (*MQTTManager)->maxInflight);
    SystemLog_info(SystemConfig->SystemLog, "daemon {%d}", (*MQTTManager)->daemon);

    if (OK != (MQTTAliyun_Signature((*MQTTManager)->productKey, (*MQTTManager)->deviceName, (*MQTTManager)->deviceSecret, clientID, username, password)))
    {
        SystemLog_error(SystemConfig->SystemLog, "MQTTAliyun_Signature filed");
        return ErrorException;
    }
    SystemLog_info(SystemConfig->SystemLog, "clientId:   {%s}", clientID);
    SystemLog_info(SystemConfig->SystemLog, "username:   {%s}", username);
    SystemLog_info(SystemConfig->SystemLog, "password:   {%s}", password);

    MQTTAsync client;
    MQTTAsync_connectOptions connectOptions = MQTTAsync_connectOptions_initializer;
    connectOptions.minRetryInterval = (*MQTTManager)->minRetryInterval;
    connectOptions.maxRetryInterval = (*MQTTManager)->maxRetryInterval;
    connectOptions.cleansession = (*MQTTManager)->cleansession;
    connectOptions.keepAliveInterval = (*MQTTManager)->keepAliveInterval;
    connectOptions.automaticReconnect = (*MQTTManager)->automaticReconnect;
    connectOptions.maxRetryInterval = (*MQTTManager)->maxRetryInterval;
    connectOptions.maxInflight = (*MQTTManager)->maxInflight;
    connectOptions.username = username;
    connectOptions.password = password;
    connectOptions.onSuccess = onConnect;
    connectOptions.onFailure = onConnectFailure;
    connectOptions.context = client;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;

    int rc;

    if ((rc = MQTTAsync_create(&client, (*MQTTManager)->serverURI, clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to create client object, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    if ((rc = MQTTAsync_setCallbacks(client, NULL, connlost, messageArrived, NULL)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to set callback, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

    if ((rc = MQTTAsync_connect(client, &connectOptions)) != MQTTASYNC_SUCCESS)
    {
        printf("Failed to start connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }

//     while (!connected)
//     {
// #if defined(_WIN32)
//         Sleep(100);
// #else
//         usleep(1000000L);
// #endif
//     }

//     while (!finished)
//     {
//         usleep(1000000L);
//         int64_t t = getTime();
//         char buf[256];
//         int n = snprintf(buf, sizeof(buf), "%lld", (long long)t);
//         printf("%s\n", buf);

//         pub_opts.onSuccess = onSend;
//         pub_opts.onFailure = onSendFailure;
//         pub_opts.context = client;

//         pubmsg.payload = buf;
//         pubmsg.payloadlen = n;
//         pubmsg.qos = QOS;
//         pubmsg.retained = 0;

//         if ((rc = MQTTAsync_sendMessage(client, TOPIC, &pubmsg, &pub_opts)) != MQTTASYNC_SUCCESS)
//         {
//             printf("Failed to start sendMessage, return code %d\n", rc);
//             exit(EXIT_FAILURE);
//         }

// #if defined(_WIN32)
//         Sleep(SAMPLE_PERIOD);
// #else
//         usleep(SAMPLE_PERIOD * 1000);
// #endif
//     }

    //MQTTAsync_destroy(&client);
    return OK;
}

void MQTTManager_cleanup(MQTTManager_t *MQTTManager, AllocateUtils_t *AllocateUtils)
{
    if (AllocateUtils)
    {
        MQTTAsync_destroy(&(MQTTManager->client));
        AllocateUtils_free(AllocateUtils, MQTTManager);
    }
}
