#include <developer/config.h>
#include <SnowWorkerM1.h>
#include <SnowFlake.h>
#include <SHA256WithRSA.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <developer/file.h>
#include <httpd.h>
#include <aliyun_iot_api.h>
#include <HMAC_SHA1.h>
#include <Base64.h>

#include <wangyonglin/elements.h>

#define MSG_QUE_KEY_ID 1996 // 消息队列标识
config_t *config;

httpd_t *httpd;
message_t *id;
ok_t ret;

mapping_arguments_t aliyun_iot_api_commands[] = {

    {"Format", "JSON", STRING, offsetof(AliPubParams_t, Format)},
    {"Version", "2020-04-20", STRING, offsetof(AliPubParams_t, Version)},
    {"AccessKeyId", NULL, STRING, offsetof(AliPubParams_t, AccessKeyId)},
    {"SignatureMethod", "HMAC-SHA1", STRING, offsetof(AliPubParams_t, SignatureMethod)},
    {"SignatureVersion", "1.0", STRING, offsetof(AliPubParams_t, SignatureVersion)},
    {"RegionId", "cn-shanghai", STRING, offsetof(AliPubParams_t, RegionId)}};
int commands_size = sizeof(aliyun_iot_api_commands) / sizeof(aliyun_iot_api_commands[0]);
AliPubParams_t *AliPubParams;

int main(int argc, char *argv[])
{
    if (ret = config_create(&config, 1024, argc, argv) != Ok)
    {
        return ret;
    }
    config->msg1 = sutpc_creatMsgQue(MSG_QUE_KEY_ID);
    SnowFlake_create(1, 1, 10);

    object_create(config->allocate, (void **)&AliPubParams, sizeof(AliPubParams_t));
    mapping_create(config->mapping, AliPubParams, "/home/wangyonglin/github/wangyonglin/conf/aliyun.conf", "ALIIOT_API_PUBLIC", aliyun_iot_api_commands, commands_size);
    // printf("\tFormat%s [%p]\r\n", AliPubParams->Format, &AliPubParams->Format);
    // printf("\tVersion%s [%p]\r\n", AliPubParams->Version, &AliPubParams->Version);
    // printf("\tAccessKeyId%s [%p]\r\n", AliPubParams->AccessKeyId, &AliPubParams->AccessKeyId);
    // printf("\tSignatureMethod%s [%p]\r\n", AliPubParams->SignatureMethod, &AliPubParams->SignatureMethod);
    // printf("\tSignatureVersion%s [%p]\r\n", AliPubParams->SignatureVersion, &AliPubParams->SignatureVersion);
    // printf("\tRegionId%s [%p]\r\n", AliPubParams->RegionId, &AliPubParams->RegionId);

    elements *arguments;
    elements_create(&arguments,20);
    elements_addstring(arguments, "Format", AliPubParams->Format, strlen(AliPubParams->Format));
    elements_addstring(arguments, "Version", AliPubParams->Version, strlen(AliPubParams->Version));
    elements_addstring(arguments, "AccessKeyId", AliPubParams->AccessKeyId, strlen(AliPubParams->AccessKeyId));
    elements_addstring(arguments, "SignatureMethod", AliPubParams->SignatureMethod, strlen(AliPubParams->SignatureMethod));
    elements_addstring(arguments, "SignatureVersion", AliPubParams->SignatureVersion, strlen(AliPubParams->SignatureVersion));
    elements_addstring(arguments, "Adtion", "Pub", strlen("Pub"));
    elements_addstring(arguments, "Bdtion", "Pub", strlen("Pub"));
    elements_addstring(arguments, "ProductKey", "ProductKey", strlen("ProductKey"));
    elements_addstring(arguments, "MessageContent", "MessageContent", strlen("MessageContent"));
    elements_addstring(arguments, "TopicFullName", "TopicFullName", strlen("TopicFullName"));
    elements_addinteger(arguments, "Qos", 0);
    struct _element *element_date;
    size_t element_count = elements_sort(arguments, &element_date);
    for (size_t i = 0; i < element_count; i++)
    {

        if (element_date[i].type == STRING)
        {
            printf("\t%s {%s}\r\n", element_date[i].name, element_date[i].content);
        }
        else if (element_date[i].type == INTEGER)
        {
            printf("\t%s {%d}\r\n", element_date[i].name, element_date[i].content);
        }
    }
    free(element_date);
    elements_delete(arguments);

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
    return ret;
}
