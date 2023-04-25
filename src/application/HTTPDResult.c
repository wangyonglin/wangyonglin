#include <HTTPDResult.h>

httpd_result_t *httpd_result_create(httpd_result_t **result, struct evhttp_request *request, void *arg)
{
    if (object_create((void **)result, sizeof(httpd_result_t)))
    {
        (*result)->request = request;

        evhttp_add_header((*result)->request->output_headers, "Content-Type", "application/json; charset=UTF-8");
        evhttp_add_header((*result)->request->output_headers, "Connection", "close");
        memset(&(*result)->params, 0x00, sizeof(struct evkeyvalq));
        if (arg)
        {
            (*result)->httpd = (HTTPDServer *)arg;
        }
        
        size_t post_size = EVBUFFER_LENGTH(request->input_buffer);
        char request_data[post_size + 1];
        bzero(request_data, sizeof(request_data));
        char *post_data = (char *)EVBUFFER_DATA(request->input_buffer);
        if (post_data && post_size > 0 && post_size < 1024)
        {
            memcpy(request_data, post_data, post_size);
            (*result)->postJSON = cJSON_Parse(request_data);
        }
        else
        {
            (*result)->postJSON = NULL;
        }
    }
    return NULL;
}

void httpd_result_delete(httpd_result_t *result)
{
    if (result->postJSON)
    {
        cJSON_free(result->postJSON);
    }
    object_delete(result);
}

void httpd_complete(httpd_result_t *result, int code, char *jsonstring)
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
            evhttp_send_reply(result->request, 200, "ok", buffer);
        }
        else
        {
            evhttp_send_reply(result->request, code, "fail", buffer);
        }

        evbuffer_free(buffer);
    }
}
void httpd_result_complete(httpd_result_t *result, ResultUtil *ResUtil)
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
            evhttp_send_reply(result->request, 200, "ok", buffer);
        else
            evhttp_send_reply(result->request, 400, "fail", buffer);
        evbuffer_free(buffer);
    }
}

// 解析http头，主要用于get请求时解析uri和请求参数
char *httpd_result_get_params(httpd_result_t *result, const char *query_char)
{
    if (result->request == NULL || query_char == NULL)
    {
        //  printf("====line:%d,%s\n", __LINE__, "input params is null.");
        return NULL;
    }
    struct evhttp_uri *decoded = NULL;
    char *query = NULL;
    char *query_result = NULL;
    const char *path;
    const char *uri = evhttp_request_get_uri(result->request); // 获取请求uri

    if (uri == NULL)
    {
        //  printf("====line:%d,evhttp_request_get_uri return null\n", __LINE__);
        return NULL;
    }
    else
    {

        //  printf("====line:%d,Got a GET request for <%s>\n", __LINE__, uri);
    }
    // 解码uri
    decoded = evhttp_uri_parse(uri);
    if (!decoded)
    {
        //    printf("====line:%d,It's not a good URI. Sending BADREQUEST\n", __LINE__);
        evhttp_send_error(result->request, HTTP_BADREQUEST, 0);
        return NULL;
    }
    // 获取uri中的path部分
    path = evhttp_uri_get_path(decoded);
    if (path == NULL)
    {
        path = "/";
    }
    else
    {
        //  printf("====line:%d,path is:%s\n", __LINE__, path);
    }
    // 获取uri中的参数部分
    query = (char *)evhttp_uri_get_query(decoded);
    if (query == NULL)
    {
        //  printf("====line:%d,evhttp_uri_get_query return null\n", __LINE__);
        return NULL;
    }
    // 查询指定参数的值
    evhttp_parse_query_str(query, &result->params);
    query_result = (char *)evhttp_find_header(&result->params, query_char);
    return query_result;
}

#define BUF_MAX 1024 * 16
// 解析post请求数据
// string_t httpd_result_post_params(httpd_result_t *result)
// {
//     string_t poststring = string_null_command;
//     size_t post_size = 0;
//     // 获取数据长度
//     post_size = evbuffer_get_length(result->request->input_buffer);

//     printf("====line:%d,post len:%d\n", __LINE__, post_size);
//     if (post_size <= 0)
//     {
//         printf("====line:%d,post msg is empty!\n", __LINE__);
//         return poststring;
//     }
//     else
//     {

//         size_t copy_len = post_size > BUF_MAX ? BUF_MAX : post_size;
//         printf("====line:%d,post len:%d, copy_len:%d\n", __LINE__, post_size, copy_len);
//         poststring = string_create(evbuffer_pullup(result->request->input_buffer, -1), copy_len);
//         // buf[post_size] = '\0';
//         //  printf("====line:%d,post msg:%s\n", __LINE__, buf);
//     }
//     return poststring;
// }
