
#include <SnowFlake.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/string.h>
#include <wangyonglin/list.h>
#include <SnowFlake.h>
#include <Base64.h>
#include <evhttp.h>
#include <HMAC_SHA1.h>
#include <curl/curl.h>
#include <application/aliapis.h>
#include <application/aliutils.h>
#include <wangyonglin/wangyonglin.h>
// 通过自定义Topic向指定设备发布消息
ok_t aliapis_https_pub(char **aliapisurl, struct _aliutils_apis_t *apis, char *MessageContentText, size_t MessageContentSize)
{
    if (!apis)
    {
        return ArgumentException;
    }

    char *PreifxSignature = NULL;
    char *Timestamp = NULL;
    aliurls_timestamp(&Timestamp);
    char *SignatureNonce = NULL;
    SnowFlakeFormat(&SignatureNonce);
    list *objlist;
    if (!list_create(&objlist, 20))
    {
        return ErrorException;
    }
    list_add_string(objlist, "Action", "Pub", strlen("Pub"));
    list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
    list_add_string(objlist, "MessageContent", MessageContentText, MessageContentSize);
    list_add_string(objlist, "TopicFullName", apis->TopicFullName, strlen(apis->TopicFullName));
    list_add_string(objlist, "Format", apis->Format, strlen(apis->Format));
    list_add_string(objlist, "Version", apis->Version, strlen(apis->Version));
    list_add_string(objlist, "AccessKeyId", apis->AccessKeyId, strlen(apis->AccessKeyId));
    list_add_string(objlist, "SignatureMethod", apis->SignatureMethod, strlen(apis->SignatureMethod));
    list_add_string(objlist, "Timestamp", Timestamp, strlen(Timestamp));
    list_add_string(objlist, "SignatureVersion", apis->SignatureVersion, strlen(apis->SignatureVersion));
    list_add_string(objlist, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
    list_add_string(objlist, "RegionId", apis->RegionId, strlen(apis->RegionId));
    list_sort(objlist);
    char *urlArgumentsString;
    aliutls_url_list(&urlArgumentsString, 1024, objlist);

    message("config->TopicFullName", apis->TopicFullName);
    message("MessageContentText", MessageContentText);

    char *base64TextUrl;
    aliurls_base64(&base64TextUrl, 1024, urlArgumentsString, apis->AccessKeySecret);

    char *urlString;
    strnull(&urlString, 1024);
    strcat(urlString, "https://iot.cn-shanghai.aliyuncs.com?");
    strcat(urlString, urlArgumentsString);
    strcat(urlString, "&Signature=");
    strcat(urlString, base64TextUrl);
    strcrt(aliapisurl, urlString, strlen(urlString));
    strdel(urlString);
    list_delete(objlist);
    strdel(base64TextUrl);
    strdel(urlArgumentsString);
    strdel(Timestamp);
    strdel(SignatureNonce);
    return OK;
}

// // void aliapisRegisterDevice(aliyun_config *config, char *ProductKey)
// // {
// //     list *list;

// //     char *Signature;
// //     char *SignatureNonce;
// //     char *Timestamp;
// //     char *PreifxSignature = NULL;
// //     if (!config)
// //     {
// //         return ArgumentException;
// //     }

// //     if (!list_create(&list, 20))
// //     {
// //         return ErrorException;
// //     }
// //     SnowFlake_IdGenerator_toString(&SignatureNonce);
// //     aliurls_timestamp(&Timestamp);
// //     // 系统规定参数。取值：Pub
// //     list_add_string(list, "Action", "RegisterDevice", strlen("RegisterDevice"));
// //     // 要接收消息的设备所属产品的ProductKey
// //     list_add_string(list, "ProductKey", ProductKey, strlen(ProductKey));
// //     /**公共请求参数
// //      * 公共请求参数是调用每个API时都需要使用的请求参数
// //      */
// //     // 返回值的类型，支持JSON和XML类型。默认为XML
// //     list_add_string(list, "Format", config->Format, strlen(config->Format));
// //     // API版本号，为日期形式：YYYY-MM-DD，最新版本为2018-01-20 。每个接口可以存在多个版本
// //     list_add_string(list, "Version", config->Version, strlen(config->Version));
// //     /**阿里云颁发给用户的访问服务所用的密钥ID。
// //           登录阿里云控制台，将光标移至账号头像上，然后单击Accesskey管理，跳转至AccessKey页面，即可创建和查看AccessKey*/
// //     list_add_string(list, "AccessKeyId", config->AccessKeyId, strlen(config->AccessKeyId));
// //     // 签名方式，目前支持HMAC-SHA1
// //     list_add_string(list, "SignatureMethod", config->SignatureMethod, strlen(config->SignatureMethod));
// //     /*请求的时间戳。日期格式按照ISO8601标准表示，并需要使用UTC时间。格式为YYYY-MM-ddTHH:mm:ssZ。
// //         例如，2016-01-04T12:00:00Z表示北京时间2016年01月04日20点0分0秒。*/
// //     list_add_string(list, "Timestamp", Timestamp, strlen(Timestamp));
// //     /**签名算法版本。目前版本是1.0*/
// //     list_add_string(list, "SignatureVersion", config->SignatureVersion, strlen(config->SignatureVersion));
// //     /**唯一随机数。用于防止网络重放攻击。用户在不同请求中要使用不同的随机数值*/
// //     list_add_string(list, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
// //     /**设备所在地域（与控制台上的地域对应），如cn-shanghai。*/
// //     list_add_string(list, "RegionId", config->RegionId, strlen(config->RegionId));
// //     list_sort(list);
// //     // string_rows("Timestamp", Timestamp);
// //     char *urlArgumentsString;
// //     aliutls_url_list(&urlArgumentsString, 1024, list);

// //     string_rows("config->TopicFullName", config->TopicFullName);

// //     char *base64TextUrl;
// //     aliurls_base64(&base64TextUrl, 1024, urlArgumentsString, config->AccessKeySecret);
// //     // string_rows("base64TextUrl", base64TextUrl);
// //     char *urlString;
// //     strnull(&urlString, 1024);
// //     strcat(urlString, "https://iot.cn-shanghai.aliyuncs.com?");
// //     strcat(urlString, urlArgumentsString);
// //     strcat(urlString, "&Signature=");
// //     strcat(urlString, base64TextUrl);
// //     //  string_rows("urlString", urlString);
// //     aliutils_https_get(urlString);
// //     strdel(urlString);
// //     list_delete(list);
// //     strdel(SignatureNonce);
// //     strdel(Timestamp);
// //     strdel(base64TextUrl);
// //     strdel(urlArgumentsString);
// //     return OK;
// // }
