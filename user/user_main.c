#include <event.h>
// for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <cjson/cJSON.h>
#include <unistd.h>
#include <string.h>
#include <mbedtls/sha256.h>
#include <mbedtls/base64.h>
#include <mbedtls/ctr_drbg.h>
/**
 * @brief   CTR_DRBG Function demo
 * @author  mculover666
 * @date    2020/09/22
 */

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <saltbytes.h>
#include <user_config.h>
#include <stdlib.h>
#include <unix/signal.h>
struct evhttp *m_phttpd;
void login_handler(struct evhttp_request *req, void *arg)
{
    //获取POST方法的数据
    size_t post_size = EVBUFFER_LENGTH(req->input_buffer);
    char request_data[1024] = {0};
    char *post_data = (char *)EVBUFFER_DATA(req->input_buffer);
    cJSON *root = NULL;
    if (post_size > 0 && post_size < 1024)
    {
        memcpy(request_data, post_data, post_size);
        root = cJSON_Parse(request_data);
    }
    //给前段回复一个响应结果
    cJSON *response_root = cJSON_CreateObject();
    if (root && response_root)
    {
        cJSON *username_obj = cJSON_GetObjectItem(root, "username");
        cJSON *password_obj = cJSON_GetObjectItem(root, "password");
        cJSON *version_obj = cJSON_GetObjectItem(root, "version");

        if (username_obj && password_obj && version_obj)
        {
            cJSON_AddStringToObject(response_root, "username", username_obj->valuestring);
            cJSON_AddStringToObject(response_root, "password", password_obj->valuestring);
            cJSON_AddStringToObject(response_root, "version", version_obj->valuestring);
        }
    }
    cJSON_AddNumberToObject(response_root, "ret", 0);
    char *response_str = cJSON_Print(response_root);
    evhttp_add_header(req->output_headers, "Content-Type", "application/json; charset=UTF-8");
    evhttp_add_header(req->output_headers, "Connection", "close");

    //输出的内容
    struct evbuffer *buf = evbuffer_new();
    evbuffer_add_printf(buf, "%s", response_str);

    //将封装好的evbuffer 发送给客户端
    evhttp_send_reply(req, HTTP_OK, "OK", buf);

    evbuffer_free(buf);

    cJSON_Delete(response_root);
    free(response_str);
}

saltbytes_t *saltbytes;
user_config_t *config;

void err_handler(int sig)
{

    if ((SIGINT == sig) | (SIGTERM == sig))
    {
        user_config_del(config);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{

    /**
      unsigned char ciphertext[512] = {0};
        if (saltbytes_init(&saltbytes, "W@ng0811") == 0)
        {
            printf("\tplaintext %s[%d]\r\n", saltbytes->plaintext, strlen(saltbytes->plaintext));
            printf("\tsalttext %s[%d]\r\n", saltbytes->salttext, strlen(saltbytes->salttext));
            saltbytes_sha256(saltbytes, ciphertext);

            printf("SHA256加密后:[%d]", strlen(ciphertext));
            for (int i = 0; i < strlen(ciphertext); i++)
            {

                printf("%02x", ciphertext[i]);
            }
            printf("\r\n");
            saltbytes_free(saltbytes);
        }
                //初始化event API
        event_init();
        //创建一个HTTPserver
        m_phttpd = evhttp_start("0.0.0.0", 80);

        //设置超时时间15秒
        evhttp_set_timeout(m_phttpd, 15);

        evhttp_set_cb(m_phttpd, "/v1/login", login_handler, NULL);
        //循环监听
        event_dispatch();
        //实际上不会释放，代码不会运行到这一步
        evhttp_free(m_phttpd);
    **/
    unix_signal_error(err_handler);
    if (user_config_crt(&config, argc, argv))
    {
        event_init();
        //创建一个HTTPserver
        m_phttpd = evhttp_start("0.0.0.0", 80);

        //设置超时时间15秒
        evhttp_set_timeout(m_phttpd, 15);

        evhttp_set_cb(m_phttpd, "/v1/login", login_handler, NULL);
        //循环监听
        event_dispatch();
        //实际上不会释放，代码不会运行到这一步
        evhttp_free(m_phttpd);
        user_config_del(config);
    }

    return EXIT_SUCCESS;
}