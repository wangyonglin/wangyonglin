#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
#include <https/https.h>
#include <mosquitto/mosquitto.h>
void https_restful_notfound(struct evhttp_request *request, void *args);
void *https_restful_dispatch(void *args);
void https_restful_get(struct evhttp_request *req, void *arg);
wangyonglin_mosquitto_publish_t publish_t;

int https_restful_appcation(wangyonglin_signal_t *signal_t)
{
    wangyonglin_socket_t socket_t;
    socket_t.port = wangyonglin_conf_int("https_port", 80);
    socket_t.backlog == wangyonglin_conf_int("https_backlog", 100);
    wangyonglin_socket_build(&socket_t);
    int nthreads = 10;
    pthread_t ths[nthreads];
    https_info info_arr[nthreads], *pinfo;
    int i, ret, opt = 1;
    for (i = 0; i < nthreads; i++)
    {
        pinfo = &info_arr[i];
        pinfo->base = event_base_new();
        if (pinfo->base == NULL)
        {
            wangyonglin_error_perror("event_base_new \n");
            return -1;
        }
        pinfo->httpd = evhttp_new(pinfo->base);
        if (pinfo->httpd == NULL)
        {
            wangyonglin_error_perror("evhttp_new \n");
            return -1;
        }
        if (evhttp_accept_socket(pinfo->httpd, socket_t.sockfd) != 0)
        {
            wangyonglin_logger_failure("evhttp_accept_socket failed! port:%d\n", socket_t.port);
            return -1;
        }
        evhttp_set_cb(pinfo->httpd, "/mosquitto", https_restful_get, signal_t);
        evhttp_set_gencb(pinfo->httpd, https_restful_notfound, 0);
        ret = pthread_create(&ths[i], NULL, https_restful_dispatch, pinfo);
    }
    for (i = 0; i < nthreads; i++)
    {
        pthread_join(ths[i], NULL);
    }

    return ret;
}
void https_restful_notfound(struct evhttp_request *request, void *args)
{
    https_response_t response;
    response.request = request;
    https_response_failure(&response, HTTP_NOTFOUND, "404 found");
    https_response_send(&response);
}
void *https_restful_dispatch(void *args)
{
    struct https_info *info = (struct https_info *)args;
    event_base_dispatch(info->base);
    event_base_free(info->base);
    evhttp_free(info->httpd);
}
//??????http??????????????????get???????????????uri???????????????
char *https_restful_params(https_response_t *response, const char *query_char)
{
    struct evkeyvalq params = {0};
    response->params = &params;
    if (response->request == NULL || response->params == NULL || query_char == NULL)
    {
        https_response_failure(&response, HTTP_BADREQUEST, "input params is null.");
        https_response_send(&response);
        return NULL;
    }
    struct evhttp_uri *decoded = NULL;
    char *query = NULL;
    char *query_result = NULL;
    const char *path;
    const char *uri = evhttp_request_get_uri(response->request); //????????????uri
    if (uri == NULL)
    {
        printf("====line:%d,evhttp_request_get_uri return null\n", __LINE__);
        return NULL;
    }
    //??????uri
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        printf("====line:%d,It's not a good URI. Sending BADREQUEST\n", __LINE__);
        evhttp_send_error(response->request, HTTP_BADREQUEST, 0);
        return NULL;
    }
    //??????uri??????path??????
    path = evhttp_uri_get_path(decoded);
    if (path == NULL)
    {
        path = "/";
    }
    //??????uri??????????????????
    query = (char *)evhttp_uri_get_query(decoded);
    if (query == NULL)
    {
        printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
        return NULL;
    }
    //????????????????????????
    evhttp_parse_query_str(query, response->params);
    query_result = (char *)evhttp_find_header(response->params, query_char);
    return query_result;
}

//??????get??????
void https_restful_get(struct evhttp_request *req, void *arg)
{
    https_response_t response;
    response.request = req;
    wangyonglin_signal_t *signal_t = (wangyonglin_signal_t *)arg;
    wangyonglin_logger_success("### IP: %s:%d CODE: %d URL: %s", req->remote_host, req->remote_port, HTTP_OK, evhttp_request_get_uri(req));
    if (response.request == NULL)
    {
        https_response_failure(&response, HTTP_BADREQUEST, "input params is null.");
        https_response_send(&response);
        return;
    }
    char *sign = NULL;
    char *topic = NULL;
    char *payload = NULL;
    sign = https_restful_params(&response, "sign"); //??????get??????uri??????sign??????
    if (sign == NULL)
    {
        https_response_failure(&response, HTTP_BADREQUEST, "request uri no param sign.");
        https_response_send(&response);
        return;
    }
    topic = https_restful_params(&response, "topic"); //??????get??????uri??????sign??????
    if (topic == NULL)
    {
        https_response_failure(&response, HTTP_BADREQUEST, "request uri no param id.");
        https_response_send(&response);
        return;
    }
    payload = https_restful_params(&response, "payload"); //??????get??????uri??????data??????
    if (payload == NULL)
    {
        https_response_failure(&response, HTTP_BADREQUEST, "request uri no param data.");
        https_response_send(&response);
        return;
    }
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "sign", sign);
    cJSON_AddStringToObject(root, "topic", topic);
    cJSON_AddStringToObject(root, "payload", payload);
    char *json = cJSON_Print(root);
    wangyonglin_string_t string_topic_t = wangyonglin_null_string;
    wangyonglin_string_t string_payload_t = wangyonglin_null_string;
    wangyonglin_string_setting(&string_topic_t, topic);
    wangyonglin_string_setting(&string_payload_t, payload);
    publish_t.topic = string_topic_t;
    publish_t.payload = string_payload_t;
    https_response_success(&response, root);
    wangyonglin_signal_queue(signal_t, SIGUSR1, 100, &publish_t);
    https_response_send(&response);
}
