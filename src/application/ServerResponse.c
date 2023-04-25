#include <ServerResponse.h>

ServerResponse *ServerResponseCreate(ServerResponse **response, zlog_category_t *log, struct evhttp_request *request, void *arg)
{
    if (ObjectCreate((void **)response, sizeof(ServerResponse)))
    {
        (*response)->log = log;
        (*response)->request = request;
        (*response)->body.valuelength = 0;
        (*response)->body.valuestring = NULL;
        evhttp_add_header((*response)->request->output_headers, "Content-Type", "application/json; charset=UTF-8");
        evhttp_add_header((*response)->request->output_headers, "Connection", "close");
        memset(&(*response)->params, 0x00, sizeof(struct evkeyvalq));
        if (arg)
        {
            (*response)->httpd = (HTTPDServer *)arg;
        }

        size_t post_size = EVBUFFER_LENGTH(request->input_buffer);
        char request_data[post_size + 1];
        bzero(request_data, sizeof(request_data));
        char *post_data = (char *)EVBUFFER_DATA(request->input_buffer);
        if (post_data && post_size > 0 && post_size < 1024)
        {
            StringexCreate(&(*response)->body, post_data, post_size);
        }
    }
    return NULL;
}

void ServerResponseDelete(ServerResponse *response)
{
    StringexDelete(response->body);
    ObjectDelete(response);
}
Boolean ServerResponseBySuccessfly(ServerResponse *response, cJSON *responseBody)
{

    Boolean err = Boolean_false;
    if (response)
    {
        struct evbuffer *buffer = evbuffer_new();
        if (buffer)
        {
            cJSON *root = NULL;
            if (root = cJSON_CreateObject())
            {

                Stringex id = stringex_null_command;
                Stringex timeutc = stringex_null_command;
                StringexTimeUTC(&timeutc);
                cJSON_AddStringToObject(root, "RequestId", id.valuestring);
                cJSON_AddNumberToObject(root, "Code", 0);
                cJSON_AddStringToObject(root, "ErrorMessage", "");
                cJSON_AddItemToObject(root, "Result", responseBody);
                cJSON_AddTrueToObject(root, "Success");

                cJSON_AddStringToObject(root, "Timestamp", timeutc.valuestring);
                char *jsonstring = cJSON_PrintUnformatted(root);
                if (jsonstring)
                {
                    evbuffer_add(buffer, jsonstring, strlen(jsonstring));
                    evhttp_send_reply(response->request, 200, "ok", buffer);
                    free(jsonstring);
                    err = Boolean_true;
                }
                StringexDelete(id);
                StringexDelete(timeutc);
                cJSON_Delete(root);
            }
            evbuffer_free(buffer);
        }
        err = Boolean_true;
    }
    return err;
}
Boolean ServerResponseByErrorMessage(ServerResponse *response, Integer responseCode, char *responseMessage)
{
    Boolean err = Boolean_false;
    if (response)
    {
        struct evbuffer *buffer = evbuffer_new();
        if (buffer)
        {
            cJSON *root = NULL;
            if (root = cJSON_CreateObject())
            {

                Stringex id = stringex_null_command;
                Stringex timeutc = stringex_null_command;
                StringexTimeUTC(&timeutc);
                cJSON_AddStringToObject(root, "RequestId", id.valuestring);
                cJSON_AddNumberToObject(root, "Code", responseCode);
                cJSON_AddStringToObject(root, "ErrorMessage", responseMessage);
                cJSON_AddItemToObject(root, "Result", cJSON_CreateObject());
                if (responseCode == 0)
                    cJSON_AddTrueToObject(root, "Success");
                else
                    cJSON_AddFalseToObject(root, "Success");
                cJSON_AddStringToObject(root, "Timestamp", timeutc.valuestring);
                char *jsonstring = cJSON_PrintUnformatted(root);
                if (jsonstring)
                {
                    evbuffer_add(buffer, jsonstring, strlen(jsonstring));
                    evhttp_send_reply(response->request, responseCode, "err", buffer);
                    free(jsonstring);
                    err = Boolean_true;
                }
                StringexDelete(id);
                StringexDelete(timeutc);
                cJSON_Delete(root);
            }
            evbuffer_free(buffer);
        }
        err = Boolean_true;
    }

    return err;
}

void ServerResponseStringComplete(ServerResponse *response, int code, char *jsonstring)
{

    struct evbuffer *buffer = evbuffer_new();
    if (buffer && jsonstring)
    {
        if (jsonstring)
        {
            evbuffer_add(buffer, jsonstring, strlen(jsonstring));
        }
        if (code == 200)
        {
            evhttp_send_reply(response->request, 200, "ok", buffer);
        }
        else
        {
            evhttp_send_reply(response->request, code, "fail", buffer);
        }

        evbuffer_free(buffer);
    }
}

void ServerResponseResultUtilComplete(ServerResponse *response, ResultUtil *ResUtil)
{
    char *datastring = ResultUtilStringify(ResUtil);
    struct evbuffer *buffer = evbuffer_new();
    if (buffer)
    {
        if (datastring)
        {
            evbuffer_add(buffer, datastring, strlen(datastring));
            free(datastring);
        }
        else
        {
            evbuffer_add(buffer, "{}", strlen("{}"));
        }

        cJSON *Code = cJSON_GetObjectItem(ResUtil->Root, "Code");
        if (Code->valueint == 0)
            evhttp_send_reply(response->request, 200, "ok", buffer);
        else
            evhttp_send_reply(response->request, 400, "fail", buffer);
        evbuffer_free(buffer);
    }
}

// 解析http头，主要用于get请求时解析uri和请求参数
Boolean ServerResponseQuery(Stringex *retvalue, ServerResponse *response, const char *query_char)
{
    Boolean err = Boolean_false;
    if (response == NULL || response->request == NULL || query_char == NULL)
    {
        return err;
    }
    struct evhttp_uri *decoded = NULL;
    char *query = NULL;
    char *query_result = NULL;
    const char *path;
    const char *uri = evhttp_request_get_uri(response->request); // 获取请求uri

    if (uri == NULL)
        return err;
    // 解码uri
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        evhttp_send_error(response->request, HTTP_BADREQUEST, 0);
        return err;
    }

    path = evhttp_uri_get_path(decoded);
    if (path == NULL)
    {
        path = "/";
    }
    // 获取uri中的参数部分
    query = (char *)evhttp_uri_get_query(decoded);
    if (query == NULL)
    {
        //  printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
        return err;
    }
    // 查询指定参数的值
    evhttp_parse_query_str(query, &response->params);
    query_result = (char *)evhttp_find_header(&response->params, query_char);

    if (StringexCreate(retvalue, query_result, strlen(query_result)))
    {
        return Boolean_true;
    }
    StringDelete(query_result);
    return err;
}

Boolean ServerResponseHeadersValue(Stringex *retvalue, ServerResponse *response, const char *query_char)
{

    struct evkeyvalq *headers = evhttp_request_get_input_headers(response->request);
    struct evkeyval *kv = headers->tqh_first;
    while (kv)
    {
        if (strcmp(query_char, kv->key) == 0)
        {
            if (StringexCreate(retvalue, kv->value, strlen(kv->value)))
            {
                return Boolean_true;
            }
        }
        kv = kv->next.tqe_next;
    }
    evhttp_clear_headers(headers);
    return Boolean_false;
}