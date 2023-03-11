
#include <SnowFlake.h>
#include <wangyonglin/conf.h>
#include <wangyonglin/string.h>
#include <wangyonglin/list.h>
#include <SnowFlake.h>
#include <Encrypt/Base64.h>
#include <evhttp.h>
#include <Encrypt/HMAC_SHA1.h>
#include <curl/curl.h>
#include <application/aliapis.h>
#include <application/aliutils.h>
#include <wangyonglin/wangyonglin.h>
#include <application/localapis.h>
#include <wangyonglin/buffer.h>
#include <wangyonglin/time.h>
#include <https/client.h>
#include <application/https.h>
// // 通过自定义Topic向指定设备发布消息
void HttpsPublish(localapis_t *aliapis, char *DeviceName, char *MessageContentText, size_t MessageContentSize)
{
    https_callback *callback;
    char *tmpContentBase64;
    ContentBase64(&tmpContentBase64, MessageContentText, MessageContentSize);
    char *tmpTimestamp;
    TimestampFormat(&tmpTimestamp);
    char *tmpSignatureNonce;
    SignatureNonceFormat(&tmpSignatureNonce);
    char *tmpTopicFullName;
    TopicFullNameFormat(&tmpTopicFullName, aliapis->ProductKey, DeviceName, "user/get");

    struct _list_t list[] = {
        list_string_command("Action", "Pub"),
        list_string_command("ProductKey", aliapis->ProductKey),
        list_string_command("MessageContent", tmpContentBase64),
        list_string_command("TopicFullName", tmpTopicFullName),
        list_string_command("Format", aliapis->Format),
        list_string_command("Version", aliapis->Version),
        list_string_command("AccessKeyId", aliapis->AccessKeyId),
        list_string_command("SignatureMethod", aliapis->SignatureMethod),
        list_string_command("Timestamp", tmpTimestamp),
        list_string_command("SignatureVersion", aliapis->SignatureVersion),
        list_string_command("SignatureNonce", tmpSignatureNonce),
        list_string_command("RegionId", aliapis->RegionId)};
    list_sort(list, list_count(list));
    char *SignatureString;
    SignatureFormat(&SignatureString, list, list_count(list), aliapis->AccessKeySecret);
    buffer_print("SignatureString", SignatureString);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, list, list_count(list), SignatureString);
    buffer_print("tmpUrlString", tmpUrlString);
    https_client(tmpUrlString, NULL, &callback);
    if (callback && callback->jsonformat)
    {
        buffer_print("callback->jsonformat", callback->jsonformat);
    }

    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);
    global_hooks.deallocate(tmpContentBase64);
    global_hooks.deallocate(tmpTimestamp);
    global_hooks.deallocate(tmpSignatureNonce);
    global_hooks.deallocate(tmpTopicFullName);
}

// // 通过自定义Topic向指定设备发布消息
// ok_t QueryDeviceInfo(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName)
// {
//     if (!apis)
//     {
//         return ArgumentException;
//     }

//     char *PreifxSignature = NULL;
//     char *Timestamp = NULL;
//     aliurls_timestamp(&Timestamp);
//     char *SignatureNonce = NULL;
//     SnowFlakeFormat(&SignatureNonce);
//     list *objlist;
//     if (!list_create(&objlist, 20))
//     {
//         return ErrorException;
//     }
//     list_add_string(objlist, "Action", "QueryDeviceInfo", strlen("QueryDeviceInfo"));
//     list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
//     list_add_string(objlist, "DeviceName", DeviceName, strlen(DeviceName));
//     list_add_string(objlist, "Format", apis->Format, strlen(apis->Format));
//     list_add_string(objlist, "Version", apis->Version, strlen(apis->Version));
//     list_add_string(objlist, "AccessKeyId", apis->AccessKeyId, strlen(apis->AccessKeyId));
//     list_add_string(objlist, "SignatureMethod", apis->SignatureMethod, strlen(apis->SignatureMethod));
//     list_add_string(objlist, "Timestamp", Timestamp, strlen(Timestamp));
//     list_add_string(objlist, "SignatureVersion", apis->SignatureVersion, strlen(apis->SignatureVersion));
//     list_add_string(objlist, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
//     list_add_string(objlist, "RegionId", apis->RegionId, strlen(apis->RegionId));
//     list_sort(objlist);
//     char *urlArgumentsString;
//     aliutls_urlcat_list(&urlArgumentsString, 1024, objlist);

//     // message("config->TopicFullName", apis->TopicFullName);
//     // message("MessageContentText", MessageContentText);

//     char *base64TextUrl;
//     aliurls_base64(&base64TextUrl, 1024, urlArgumentsString, apis->AccessKeySecret);

//     char *urlString;
//     strnull(&urlString, 1024);
//     strcat(urlString, "https://iot.cn-shanghai.aliyuncs.com?");
//     strcat(urlString, urlArgumentsString);
//     strcat(urlString, "&Signature=");
//     strcat(urlString, base64TextUrl);
//     strcrt(aliapisurl, urlString, strlen(urlString));
//     strdel(urlString);
//     list_delete(objlist);
//     strdel(base64TextUrl);
//     strdel(urlArgumentsString);
//     strdel(Timestamp);
//     strdel(SignatureNonce);
//     return OK;
// }

// // 通过自定义Topic向指定设备发布消息
// ok_t RegisterDevice(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName)
// {
//     if (!apis)
//     {
//         return ArgumentException;
//     }

//     char *PreifxSignature = NULL;
//     char *Timestamp = NULL;
//     aliurls_timestamp(&Timestamp);
//     char *SignatureNonce = NULL;
//     SnowFlakeFormat(&SignatureNonce);

//     list *objlist;
//     if (!list_create(&objlist, 20))
//     {
//         return ErrorException;
//     }
//     list_add_string(objlist, "Action", "RegisterDevice", strlen("RegisterDevice"));
//     list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
//     list_add_string(objlist, "DeviceName", DeviceName, strlen(DeviceName));
//     list_add_string(objlist, "Format", apis->Format, strlen(apis->Format));
//     list_add_string(objlist, "Version", apis->Version, strlen(apis->Version));
//     list_add_string(objlist, "AccessKeyId", apis->AccessKeyId, strlen(apis->AccessKeyId));
//     list_add_string(objlist, "SignatureMethod", apis->SignatureMethod, strlen(apis->SignatureMethod));
//     list_add_string(objlist, "Timestamp", Timestamp, strlen(Timestamp));
//     list_add_string(objlist, "SignatureVersion", apis->SignatureVersion, strlen(apis->SignatureVersion));
//     list_add_string(objlist, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
//     list_add_string(objlist, "RegionId", apis->RegionId, strlen(apis->RegionId));
//     list_sort(objlist);
//     char *urlArgumentsString;
//     aliutls_urlcat_list(&urlArgumentsString, 1024, objlist);

//     // message("config->TopicFullName", apis->TopicFullName);
//     // message("MessageContentText", MessageContentText);

//     char *base64TextUrl;
//     aliurls_base64(&base64TextUrl, 1024, urlArgumentsString, apis->AccessKeySecret);

//     char *urlString;
//     strnull(&urlString, 1024);
//     strcat(urlString, "https://iot.cn-shanghai.aliyuncs.com?");
//     strcat(urlString, urlArgumentsString);
//     strcat(urlString, "&Signature=");
//     strcat(urlString, base64TextUrl);
//     strcrt(aliapisurl, urlString, strlen(urlString));
//     strdel(urlString);
//     list_delete(objlist);
//     strdel(base64TextUrl);
//     strdel(urlArgumentsString);
//     strdel(Timestamp);
//     strdel(SignatureNonce);
//     return OK;
// }

// ok_t QueryDeviceStatistics(char **aliapisurl, struct _aliutils_apis_t *apis)
// {
//     if (!apis)
//     {
//         return ArgumentException;
//     }

//     char *PreifxSignature = NULL;
//     char *Timestamp = NULL;
//     aliurls_timestamp(&Timestamp);
//     char *SignatureNonce = NULL;
//     SnowFlakeFormat(&SignatureNonce);

//     list *objlist;
//     if (!list_create(&objlist, 20))
//     {
//         return ErrorException;
//     }
//     list_add_string(objlist, "Action", "QueryDeviceStatistics", strlen("QueryDeviceStatistics"));
//     list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
//     list_add_string(objlist, "Format", apis->Format, strlen(apis->Format));
//     list_add_string(objlist, "Version", apis->Version, strlen(apis->Version));
//     list_add_string(objlist, "AccessKeyId", apis->AccessKeyId, strlen(apis->AccessKeyId));
//     list_add_string(objlist, "SignatureMethod", apis->SignatureMethod, strlen(apis->SignatureMethod));
//     list_add_string(objlist, "Timestamp", Timestamp, strlen(Timestamp));
//     list_add_string(objlist, "SignatureVersion", apis->SignatureVersion, strlen(apis->SignatureVersion));
//     list_add_string(objlist, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
//     list_add_string(objlist, "RegionId", apis->RegionId, strlen(apis->RegionId));
//     list_sort(objlist);
//     char *urlArgumentsString;
//     aliutls_urlcat_list(&urlArgumentsString, 1024, objlist);

//     // message("config->TopicFullName", apis->TopicFullName);
//     // message("MessageContentText", MessageContentText);

//     char *base64TextUrl;
//     aliurls_base64(&base64TextUrl, 1024, urlArgumentsString, apis->AccessKeySecret);

//     char *urlString;
//     strnull(&urlString, 1024);
//     strcat(urlString, "https://iot.cn-shanghai.aliyuncs.com?");
//     strcat(urlString, urlArgumentsString);
//     strcat(urlString, "&Signature=");
//     strcat(urlString, base64TextUrl);
//     strcrt(aliapisurl, urlString, strlen(urlString));
//     strdel(urlString);
//     list_delete(objlist);
//     strdel(base64TextUrl);
//     strdel(urlArgumentsString);
//     strdel(Timestamp);
//     strdel(SignatureNonce);
//     return OK;
// }

// ok_t GetDeviceStatus(char **aliapisurl, struct _aliutils_apis_t *apis, char *IotId)
// {
//     if (!apis)
//     {
//         return ArgumentException;
//     }

//     char *PreifxSignature = NULL;
//     char *Timestamp = NULL;
//     aliurls_timestamp(&Timestamp);
//     char *SignatureNonce = NULL;
//     SnowFlakeFormat(&SignatureNonce);

//     list *objlist;
//     if (!list_create(&objlist, 20))
//     {
//         return ErrorException;
//     }
//     list_add_string(objlist, "Action", "GetDeviceStatus", strlen("GetDeviceStatus"));
//     list_add_string(objlist, "IotId", IotId, strlen(IotId));
//     list_add_string(objlist, "Format", apis->Format, strlen(apis->Format));
//     list_add_string(objlist, "Version", apis->Version, strlen(apis->Version));
//     list_add_string(objlist, "AccessKeyId", apis->AccessKeyId, strlen(apis->AccessKeyId));
//     list_add_string(objlist, "SignatureMethod", apis->SignatureMethod, strlen(apis->SignatureMethod));
//     list_add_string(objlist, "Timestamp", Timestamp, strlen(Timestamp));
//     list_add_string(objlist, "SignatureVersion", apis->SignatureVersion, strlen(apis->SignatureVersion));
//     list_add_string(objlist, "SignatureNonce", SignatureNonce, strlen(SignatureNonce));
//     list_add_string(objlist, "RegionId", apis->RegionId, strlen(apis->RegionId));
//     list_sort(objlist);
//     char *urlArgumentsString;
//     aliutls_urlcat_list(&urlArgumentsString, 1024, objlist);

//     // message("config->TopicFullName", apis->TopicFullName);
//     // message("MessageContentText", MessageContentText);

//     char *base64TextUrl;
//     aliurls_base64(&base64TextUrl, 1024, urlArgumentsString, apis->AccessKeySecret);

//     char *urlString;
//     strnull(&urlString, 1024);
//     strcat(urlString, "https://iot.cn-shanghai.aliyuncs.com?");
//     strcat(urlString, urlArgumentsString);
//     strcat(urlString, "&Signature=");
//     strcat(urlString, base64TextUrl);
//     strcrt(aliapisurl, urlString, strlen(urlString));
//     strdel(urlString);
//     list_delete(objlist);
//     strdel(base64TextUrl);
//     strdel(urlArgumentsString);
//     strdel(Timestamp);
//     strdel(SignatureNonce);
//     return OK;
// }