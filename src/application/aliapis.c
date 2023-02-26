
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
ok_t Pub(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName, char *MessageContentText, size_t MessageContentSize)
{
    if (!apis)
    {
        return ArgumentException;
    }
    size_t ProductKeySize = strlen(apis->ProductKey);
    size_t DeviceNameSize = strlen(DeviceName);

    char *PreifxSignature = NULL;
    char *Timestamp = NULL;
    aliurls_timestamp(&Timestamp);
    char *SignatureNonce = NULL;
    SnowFlakeFormat(&SignatureNonce);
    char *TopicFullName;
    strcrt(&TopicFullName, NULL, ProductKeySize + DeviceNameSize + strlen(apis->TopicFullName)+8);
    sprintf(TopicFullName, "/%s/%s%s", apis->ProductKey, DeviceName, apis->TopicFullName);
    list *objlist;
    if (!list_create(&objlist, 20))
    {
        return ErrorException;
    }
    list_add_string(objlist, "Action", "Pub", strlen("Pub"));
    list_add_string(objlist, "ProductKey", apis->ProductKey, ProductKeySize);
    list_add_string(objlist, "MessageContent", MessageContentText, MessageContentSize);
    list_add_string(objlist, "TopicFullName", TopicFullName, strlen(TopicFullName));
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

    message("TopicFullName", TopicFullName);
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
    strdel(TopicFullName);
    return OK;
}

// 通过自定义Topic向指定设备发布消息
ok_t QueryDeviceInfo(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName)
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
    list_add_string(objlist, "Action", "QueryDeviceInfo", strlen("QueryDeviceInfo"));
    list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
    list_add_string(objlist, "DeviceName", DeviceName, strlen(DeviceName));
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

    // message("config->TopicFullName", apis->TopicFullName);
    // message("MessageContentText", MessageContentText);

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

// 通过自定义Topic向指定设备发布消息
ok_t RegisterDevice(char **aliapisurl, struct _aliutils_apis_t *apis, char *DeviceName)
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
    list_add_string(objlist, "Action", "RegisterDevice", strlen("RegisterDevice"));
    list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
    list_add_string(objlist, "DeviceName", DeviceName, strlen(DeviceName));
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

    // message("config->TopicFullName", apis->TopicFullName);
    // message("MessageContentText", MessageContentText);

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

ok_t QueryDeviceStatistics(char **aliapisurl, struct _aliutils_apis_t *apis)
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
    list_add_string(objlist, "Action", "QueryDeviceStatistics", strlen("QueryDeviceStatistics"));
    list_add_string(objlist, "ProductKey", apis->ProductKey, strlen(apis->ProductKey));
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

    // message("config->TopicFullName", apis->TopicFullName);
    // message("MessageContentText", MessageContentText);

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

ok_t GetDeviceStatus(char **aliapisurl, struct _aliutils_apis_t *apis, char *IotId)
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
    list_add_string(objlist, "Action", "GetDeviceStatus", strlen("GetDeviceStatus"));
    list_add_string(objlist, "IotId", IotId, strlen(IotId));
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

    // message("config->TopicFullName", apis->TopicFullName);
    // message("MessageContentText", MessageContentText);

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