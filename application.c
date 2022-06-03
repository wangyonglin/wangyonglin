#include <wangyonglin/config.h>
#include <wangyonglin/log.h>
#include <event.h>
// for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <unistd.h>
#include <https/conf.h>
#include <cjson/cJSON_Response.h>
#include "MQTTClient.h"
#include <mqtt/config.h>
#include <https/config.h>

#include <string/string.h>
#define QOS 1
#define TIMEOUT 10000L

ok_t message_handler(config_t *config, const char *addr, char *id, const char *usr, const char *pw, const char *topicName, void *payload, int payloadlen, int *rc)
{
    MQTTClient client;
    MQTTClient_connectOptions options = MQTTClient_connectOptions_initializer;
    MQTTClient_message message = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;

    MQTTClient_create(&client, addr, id, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    options.keepAliveInterval = 20;
    options.cleansession = 1;
    options.username = usr;
    options.password = pw;

    if ((*rc = MQTTClient_connect(client, &options)) == MQTTCLIENT_SUCCESS)
    {

        message.payload = payload;
        message.payloadlen = payloadlen;
        message.qos = QOS;
        message.retained = 0;
        MQTTClient_publishMessage(client, topicName, &message, &token);
        /*
            log_write(cfmqtt->config, LOG_INFO, "Waiting for up to %d seconds for publication of %s\n"
                                            "on topic %s for client with ClientID: %s\n",
                  (int)(TIMEOUT / 1000), PAYLOAD, TOPIC, CLIENTID);
        */
        *rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
        // printf("Message with delivery token %d delivered\n", token);
        MQTTClient_disconnect(client, 10000);
        MQTTClient_destroy(&client);
        if (*rc == 0)
            return ok;
        else
            return failed;
    }
    return failed;
}

void login_handler(struct evhttp_request *req, void *arg)
{
    const char *uri = evhttp_request_get_uri(req);
    struct evhttp_uri *decoded = evhttp_uri_parse(uri);
    const char *topic = evhttp_uri_get_path(decoded);
    cJSON *root = NULL;
    mqtt_config_t *mconfig = (mqtt_config_t *)arg;
    char *id;
    string_rand_crt(&id, 16);
    cJSON_Response_t *obj = NULL;
    cJSON_Response(&obj, 200, "ok");
    char *buffer_data;
    char *jdat;
    if (string_crt(&buffer_data, (char *)EVBUFFER_DATA(req->input_buffer), EVBUFFER_LENGTH(req->input_buffer)))
    {
        int rc;
        root = cJSON_Parse(buffer_data);

        if (root && (jdat = cJSON_Print(root)))
        {
            string_del(buffer_data);
            if (mconfig->message(mconfig->config, mconfig->conf->address, id, mconfig->conf->username, mconfig->conf->password, topic, jdat, strlen(jdat), &rc) == ok)
            {
                cJSON_Response_msg(obj, rc, "MQTT连接成功");
                cJSON_AddItemToObject(obj->context, "request_data_json", root);
            }
            else
            {
                cJSON_Response_msg(obj, rc, "MQTT连接失败[错误码%d]", rc);
            }

            string_del(id);
        }
        else
        {
            cJSON_Response_msg(obj, -2, "cJSON解析失败[数据不能为空]");
        }
    }
    else
    {
        cJSON_Response_msg(obj, 0, "POST 数据不能为空[错误码]");
    }
    char *out;
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");
    cJSON_Response_out(obj, &out);
    struct evbuffer *evbuf = evbuffer_new();
    evbuffer_add_printf(evbuf, out);
    evhttp_send_reply(req, HTTP_OK, "OK", evbuf);
    evbuffer_free(evbuf);
    cJSON_Response_free(obj);
    string_del(out);
    cJSON_free(root);
    return;
}
void test_handler(struct evhttp_request *req, void *arg)
{
    const char *uri = evhttp_request_get_uri(req);
    /* Decode the URI */
    struct evhttp_uri *decoded = evhttp_uri_parse(uri);
    if (decoded)
    {
        printf("It's not a good URI. Sending BADREQUEST\n");
        evhttp_send_error(req, HTTP_BADREQUEST, 0);
        return;
    }
    const char *path = evhttp_uri_get_path(decoded);
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, path);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}
void error_handler(struct evhttp_request *req, void *arg)
{
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");
    cJSON_Response_t *response = NULL;
    char *out;
    cJSON_Response(&response, 404, "404 not Found");
    cJSON_Response_out(response, &out);
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, out);
    evhttp_send_reply(req, HTTP_NOTFOUND, "404 not Found", buf);
    evbuffer_free(buf);
    cJSON_Response_free(response);
    string_del(out);
}
void other_handler(struct evhttp_request *req, void *arg)
{
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");
    cJSON_Response_t *response = NULL;
    char *out;
    cJSON_Response(&response, 400, "400 Bad Request");
    cJSON_Response_out(response, &out);
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, out);
    evhttp_send_reply(req, HTTP_BADREQUEST, "invalid http request was made", buf);
    evbuffer_free(buf);
    cJSON_Response_free(response);
    string_del(out);
}
ok_t application(config_t *config, void *args)
{

    https_config_t *hconfig;
    mqtt_config_t *mconfig;
    if (https_config_init(config, &hconfig) && mqtt_config_init(config, &mconfig))
    {
        mqtt_config_message_cb(mconfig, message_handler);
        event_init();
        //创建一个HTTPserver
        hconfig->httpd = evhttp_start(hconfig->conf->address, hconfig->conf->port);

        //设置超时时间15秒
        evhttp_set_timeout(hconfig->httpd, (int)hconfig->conf->timeout_in_secs);
        evhttp_set_gencb(hconfig->httpd, other_handler, mconfig);
        evhttp_set_cb(hconfig->httpd, "/v1/login", login_handler, mconfig);
        evhttp_set_cb(hconfig->httpd, "/test", test_handler, mconfig);
        evhttp_set_gencb(hconfig->httpd, error_handler, NULL);
        //循环监听
        event_dispatch();
        //实际上不会释放，代码不会运行到这一步
        evhttp_free(hconfig->httpd);
        https_config_free(hconfig);
        mqtt_config_free(mconfig);
    }

    return ok;
}