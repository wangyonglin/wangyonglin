#include <aliiot.h>
#include <developer/allocate.h>
#include <aliiot_signature.h>
conf_command_t aliiot_commands[] = {
    {"serverURI", String, offsetof(aliiot_t, serverURI)},
    {"productKey", String, offsetof(aliiot_t, productKey)},
    {"deviceName", String, offsetof(aliiot_t, deviceName)},
    {"deviceSecret", String, offsetof(aliiot_t, deviceSecret)},
    {"minRetryInterval", Integer, offsetof(aliiot_t, minRetryInterval)},
    {"maxRetryInterval", Integer, offsetof(aliiot_t, maxRetryInterval)},
    {"cleansession", Integer, offsetof(aliiot_t, cleansession)},
    {"automaticReconnect", Integer, offsetof(aliiot_t, automaticReconnect)},
    {"keepAliveInterval", Integer, offsetof(aliiot_t, keepAliveInterval)},
    {"maxInflight", Integer, offsetof(aliiot_t, maxInflight)}};

int aliiot_commands_size = sizeof(aliiot_commands) / sizeof(aliiot_commands[0]);

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

ok_t aliiot_initializing(aliiot_t **aliiot, program_t *program)
{
    if (!program && !program->conf && !program->logger && !program->allocate)
    {
        return ArgumentException;
    }
    if (object_create(program->allocate, (void **)aliiot, sizeof(aliiot_t)) != Ok)
    {
        logerr(program->logger, "aliiot_initializing failed");
        return NullPointerException;
    }

    if (conf_mapping(program->conf, (void **)aliiot, sizeof(aliiot_t), "ALIIOT", aliiot_commands, aliiot_commands_size) != Ok)
    {
        logerr(program->logger, "aliiot_initializing conf_mapping   failed");
        return ErrorException;
    }

    logerr(program->logger, "serverURI   {%s}", (*aliiot)->serverURI);
    logerr(program->logger, "productKey  {%s}", (*aliiot)->productKey);
    logerr(program->logger, "deviceName  {%s}", (*aliiot)->deviceName);
    logerr(program->logger, "deviceSecret    {%s}", (*aliiot)->deviceSecret);
    logerr(program->logger, "minRetryInterval    {%d}", (*aliiot)->minRetryInterval);
    logerr(program->logger, "maxRetryInterval    {%d}", (*aliiot)->maxRetryInterval);
    logerr(program->logger, "cleansession    {%d}", (*aliiot)->cleansession);
    logerr(program->logger, "automaticReconnect  {%d}", (*aliiot)->automaticReconnect);
    logerr(program->logger, "keepAliveInterval  {%d}", (*aliiot)->keepAliveInterval);
    logerr(program->logger, "maxInflight {%d}", (*aliiot)->maxInflight);

    if ((aliiot_signature((*aliiot)->productKey, (*aliiot)->deviceName, (*aliiot)->deviceSecret, clientID, username, password)) != Ok)
    {
        logerr(program->logger, "aliiot_initializing aliiot_signature   failed");
        return ErrorException;
    }
    logerr(program->logger, "clientId:   {%s}", clientID);
    logerr(program->logger, "username:   {%s}", username);
    logerr(program->logger, "password:   {%s}", password);

    MQTTAsync client;
    MQTTAsync_connectOptions connectOptions = MQTTAsync_connectOptions_initializer;
    connectOptions.minRetryInterval = (*aliiot)->minRetryInterval;
    connectOptions.maxRetryInterval = (*aliiot)->maxRetryInterval;
    connectOptions.cleansession = (*aliiot)->cleansession;
    connectOptions.keepAliveInterval = (*aliiot)->keepAliveInterval;
    connectOptions.automaticReconnect = (*aliiot)->automaticReconnect;
    connectOptions.maxRetryInterval = (*aliiot)->maxRetryInterval;
    connectOptions.maxInflight = (*aliiot)->maxInflight;
    connectOptions.username = username;
    connectOptions.password = password;
    connectOptions.onSuccess = onConnect;
    connectOptions.onFailure = onConnectFailure;
    connectOptions.context = client;
    MQTTAsync_message pubmsg = MQTTAsync_message_initializer;
    MQTTAsync_responseOptions pub_opts = MQTTAsync_responseOptions_initializer;

    int rc;

    if ((rc = MQTTAsync_create(&client, (*aliiot)->serverURI, clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTASYNC_SUCCESS)
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

    // MQTTAsync_destroy(&client);
    return Ok;
}

void aliiot_cleanup(aliiot_t *aliiot, program_t *program)
{
    if (program)
    {
        MQTTAsync_destroy(&(aliiot->client));
    }
}
