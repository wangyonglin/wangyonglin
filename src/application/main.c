#include <developer/config.h>
#include <SnowWorkerM1.h>
#include <SnowFlake.h>
#include <SHA256WithRSA.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <developer/file.h>
#include <httpd.h>
#include <aliyun_api.h>
#include <HMAC_SHA1.h>
#include <Base64.h>

#include <wangyonglin/listitem.h>
#include <wangyonglin/conf.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识
config_t *config;

httpd_t *httpd;
message_t *id;
ok_t ret;

int main(int argc, char *argv[])
{
    if (ret = config_create(&config, 1024, argc, argv) != Ok)
    {
        return ret;
    }
    SnowFlake_create(1, 1, 10);
    aliyun_config *arguments;
    aliyun_config_init(&arguments, "/home/wangyonglin/github/wangyonglin/conf/aliyun.conf");
    aliyun_config_publish(arguments, "wang", 4);
    free(arguments);
    //     // deallocate(AliPubParams);
    // httpd_initializing(&httpd, config, "/home/wangyonglin/github/wangyonglin/conf/httpd.conf");
    // httpd_start(httpd);
    // httpd_destroy(httpd);
    //     config_destroy(config);
    // char sec_key[] = "asdfgh";
    // char data[] = "jkluiop";
    // char encMessage[256] = {0};
    // int encMessageLength = sizeof(encMessage);
    // char *base64Text;
    // hmac_sha1(sec_key, strlen(sec_key), data, strlen(data), encMessage, &encMessageLength);
    // char *out = base64encode(encMessage, encMessageLength);
    // printf("\t%s\r\n", out);
    // printf("len: %d\n", strlen(out));
    // free(out);

    config_destroy(config);
    return ret;
}
