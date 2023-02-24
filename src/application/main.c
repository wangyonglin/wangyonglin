#include <wangyonglin/application.h>
#include <SnowWorkerM1.h>
#include <SnowFlake.h>
#include <SHA256WithRSA.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wangyonglin/file.h>
#include <httpd.h>
#include <aliapis.h>
#include <aliutils.h>
#include <HMAC_SHA1.h>
#include <Base64.h>

#include <wangyonglin/list.h>
#include <wangyonglin/conf.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识
app_t *app;

httpd_t *httpd;
message_t *id;
ok_t ret;

int main(int argc, char *argv[])
{
    printf("this is main\n");

    if (!application_create(&app, argc, argv))
    {
        return -1;
    }
    // logerr(app->log, "ddfsdfsfw");

    // SnowFlake_create(1, 1, 10);
    // aliutils_sys *arguments;
    // aliutils_sys_init(&arguments, "/home/wangyonglin/github/wangyonglin/conf/aliyun.conf");
    // string_rows("urls", arguments->AccessKeyId);

    // char *url;
    // aliapisPub(&url, arguments, "wang", 4);
    // string_rows("urls", url);
    // strdel(url);
    // objdel(arguments);
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

    application_delete(app);
    return ret;
}
