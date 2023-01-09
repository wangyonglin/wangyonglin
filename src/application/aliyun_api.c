#include <aliyun_api.h>
#include <SnowFlake.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/string.h>
#include <wangyonglin/listitem.h>
#include <SnowFlake.h>
#include <Base64.h>
#include <evhttp.h>
ok_t aliyun_config_init(aliyun_config **config, const char *filename)
{
    ok_t ret;
    if (!object_create((void **)config, sizeof(aliyun_config)))
    {
        return NullPointerException;
    }
    conf_command commands[] = {
        {"AccessKeyId", NULL, STRING, offsetof(aliyun_config, AccessKeyId)},
        {"AccessKeySecret", NULL, STRING, offsetof(aliyun_config, AccessKeySecret)},
        {"ProductKey", "cn-shanghai", STRING, offsetof(aliyun_config, ProductKey)},
        {"DeviceName", "JSON", STRING, offsetof(aliyun_config, DeviceName)},
        {"Format", "JSON", STRING, offsetof(aliyun_config, Format)},
        {"Version", "2020-04-20", STRING, offsetof(aliyun_config, Version)},
        {"AccessKeyId", NULL, STRING, offsetof(aliyun_config, AccessKeyId)},
        {"SignatureMethod", "HMAC-SHA1", STRING, offsetof(aliyun_config, SignatureMethod)},
        {"SignatureVersion", "1.0", STRING, offsetof(aliyun_config, SignatureVersion)},
        {"RegionId", "cn-shanghai", STRING, offsetof(aliyun_config, RegionId)},
        {"TopicFullName", NULL, STRING, offsetof(aliyun_config, TopicFullName)}};
    size_t commands_size = conf_command_size(commands);
    // printf("\t%d\r\n", commands_size);
    ret = conf_create((void **)*config, filename, NULL, commands, commands_size);

    return ret;
}
// 通过自定义Topic向指定设备发布消息
ok_t aliyun_config_publish(aliyun_config *config, char *MessageContentText, size_t MessageContentSize)
{
    listitem *list;
    char *Signature;
    char *SignatureNonce;
    char *Timestamp;
    char *PreifxSignature = NULL;

    if (!config)
    {
        return ArgumentException;
    }

    if (!listitem_create(&list, 20))
    {
        return ErrorException;
    }
    SnowFlake_IdGenerator_toString(&SignatureNonce);
    string_localtime_create(&Timestamp);
    // 系统规定参数。取值：Pub
    listitem_add_string(list, "Action", "Pub", strlen("Pub"));
    // 要接收消息的设备所属产品的ProductKey
    listitem_add_string(list, "ProductKey", config->ProductKey, strlen(config->ProductKey));
    /**要发送的消息主体
        您需要将消息原文转换成二进制数据，并进行Base64编码，从而生成消息主体*/
    listitem_add_string(list, "MessageContent", MessageContentText, MessageContentSize);
    listitem_add_string(list, "TopicFullName", config->TopicFullName, strlen(config->TopicFullName));
    /**公共请求参数
     * 公共请求参数是调用每个API时都需要使用的请求参数
     */
    // 返回值的类型，支持JSON和XML类型。默认为XML
    listitem_add_string(list, "Format", config->Format, strlen(config->Format));
    // API版本号，为日期形式：YYYY-MM-DD，最新版本为2018-01-20 。每个接口可以存在多个版本
    listitem_add_string(list, "Version", config->Version, strlen(config->Version));
    /**阿里云颁发给用户的访问服务所用的密钥ID。
          登录阿里云控制台，将光标移至账号头像上，然后单击Accesskey管理，跳转至AccessKey页面，即可创建和查看AccessKey*/
    listitem_add_string(list, "AccessKeyId", config->AccessKeyId, strlen(config->AccessKeyId));
    // 签名方式，目前支持HMAC-SHA1
    listitem_add_string(list, "SignatureMethod", config->SignatureMethod, strlen(config->SignatureMethod));
    /*请求的时间戳。日期格式按照ISO8601标准表示，并需要使用UTC时间。格式为YYYY-MM-ddTHH:mm:ssZ。
        例如，2016-01-04T12:00:00Z表示北京时间2016年01月04日20点0分0秒。*/
    listitem_add_string(list, "Timestamp", Timestamp, strlen(Timestamp));
    /**签名算法版本。目前版本是1.0*/
    listitem_add_string(list, "SignatureVersion", config->SignatureVersion, strlen(config->SignatureVersion));
    /**唯一随机数。用于防止网络重放攻击。用户在不同请求中要使用不同的随机数值*/
    listitem_add_string(list, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
    /**设备所在地域（与控制台上的地域对应），如cn-shanghai。*/
    listitem_add_string(list, "RegionId", config->RegionId, strlen(config->RegionId));

    listitem_sort(list);
    char out[125] = {0};
    char text[1024] = {0};
    int size_c = 0;
    for (size_t i = 0; i < list->items_pos; i++)
    {

        if (list->items[i].type == STRING)
        {
            //  printf("\t%s {%s}\r\n", list->items[i].name, list->items[i].data);
            sprintf(out, "%s=%s", list->items[i].name, list->items[i].data);
        }
        else if (list->items[i].type == INTEGER)
        {
            //  printf("\t%s {%d}\r\n", list->items[i].name, list->items[i].data);
            sprintf(out, "%s=%d", list->items[i].name, list->items[i].data);
        }
        strncat(text, out, strlen(out));
        if ( i + 1 < list->items_pos)
        {
            strncat(text, "&", strlen("&"));
        }
    }
    printf("\t%s\r\n", text);
    listitem_delete(list);
    string_delete(SignatureNonce);
    string_delete(Timestamp);

    return OK;
}

// size_t aliyun_signature_prefix(const char *prefix, struct _element *element_date, size_t element_count, char **pointer)
// {

//     char outbuf[2048] = {0};
//     size_t len = 0;
//     char out[128];
//     for (size_t i = 0; i < element_count; i++)
//     {
//         bzero(out, sizeof(out));
//         if (i == 0)
//         {
//             if (element_date[i].type == STRING)
//             {

//                 len += sprintf(out, "%s/%s=%s", prefix, element_date[i].name, element_date[i].content);
//             }
//             else if (element_date[i].type == INTEGER)
//             {
//                 len += sprintf(out, "%s/%s=%d", prefix, element_date[i].name, element_date[i].content);
//             }
//         }
//         else
//         {
//             if (element_date[i].type == STRING)
//             {
//                 len += sprintf(out, "&%s=%s", element_date[i].name, element_date[i].content);
//             }
//             else if (element_date[i].type == INTEGER)
//             {
//                 len += sprintf(out, "&%s=%d", element_date[i].name, element_date[i].content);
//             }
//         }
//         strncat(outbuf, out, strlen(out));
//     }
//     // printf("\t%s\r\n", outbuf);
//     string_url_encode(outbuf, strlen(outbuf), pointer, 2048);
//     // if((*pointer)) return strlen((*pointer));
//     return -1;
// }