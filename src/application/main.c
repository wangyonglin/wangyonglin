#include <wangyonglin/wangyonglin.h>
#include <SnowWorkerM1.h>
#include <SnowFlake.h>
#include <SHA256WithRSA.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wangyonglin/file.h>
#include <application/httpd.h>
#include <application/aliapis.h>
#include <application/aliutils.h>
#include <HMAC_SHA1.h>
#include <Base64.h>

#include <wangyonglin/list.h>
#include <wangyonglin/conf.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识


message_t *id;
ok_t ret;
app_t *app;
struct _command aliyun_commands[] = {{"AccessKeyId", NULL, STRING, offsetof(struct _aliutils_apis_t, AccessKeyId)},
                                     {"AccessKeySecret", NULL, STRING, offsetof(struct _aliutils_apis_t, AccessKeySecret)},
                                     {"ProductKey", "cn-shanghai", STRING, offsetof(struct _aliutils_apis_t, ProductKey)},
                                     {"DeviceName", "JSON", STRING, offsetof(struct _aliutils_apis_t, DeviceName)},
                                     {"Format", "JSON", STRING, offsetof(struct _aliutils_apis_t, Format)},
                                     {"Version", "2020-04-20", STRING, offsetof(struct _aliutils_apis_t, Version)},
                                     {"AccessKeyId", NULL, STRING, offsetof(struct _aliutils_apis_t, AccessKeyId)},
                                     {"SignatureMethod", "HMAC-SHA1", STRING, offsetof(struct _aliutils_apis_t, SignatureMethod)},
                                     {"SignatureVersion", "1.0", STRING, offsetof(struct _aliutils_apis_t, SignatureVersion)},
                                     {"RegionId", "cn-shanghai", STRING, offsetof(struct _aliutils_apis_t, RegionId)},
                                     {"TopicFullName", NULL, STRING, offsetof(struct _aliutils_apis_t, TopicFullName)},
                                     null_command};
int main(int argc, char *argv[])
{
    application_create(&app, argc, argv);
    SnowFlakeInit(1, 1, 10);

    httpd_create(app);
    httpd_start();
    // struct _aliutils_apis_t *apis;
    // objcrt(&apis, sizeof(struct _aliutils_apis_t));
    // command_init(app, apis, aliyun_commands, "ALIIOT");

    // char *url;
    // aliapis_https_pub(&url, apis, "wang", 4);
    // message("urls", url);
    // aliutils_https_get(url);

    // strdel(url);
    // objdel(apis);
    // objdel(arguments);
    //     // deallocate(AliPubParams);
    // httpd_initializing(&httpd, config, "/home/wangyonglin/github/wangyonglin/conf/httpd.conf");
    // httpd_start(httpd);
    // httpd_destroy(httpd);
    //     config_destroy(config);
   
    httpd_delete();
    return ret;
}
