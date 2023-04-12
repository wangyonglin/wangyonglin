
#include <SnowFlake.h>
#include <wangyonglin/list.h>
#include <SnowFlake.h>
#include <base64.h>
#include <evhttp.h>
#include <HMAC_SHA1.h>
#include <curl/curl.h>
#include <AliyunHandler.h>
#include <AliyunUtils.h>
#include <wangyonglin/wangyonglin.h>
#include <AliyunConfig.h>
#include <https_get.h>
#include <string_by_timestamp.h>
#include <AliyunHttps.h>
#include <string_by_id.h>
// // // 通过自定义Topic向指定设备发布消息
string_by_t HTTPSAliyunPub(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName, char *MessageContentText, size_t MessageContentSize)
{
  
    https_result *result;
    https_result_create(&result);
    char *tmpContentBase64;
    ContentBase64(&tmpContentBase64, MessageContentText, MessageContentSize);
    string_by_t utcTimestamp = string_null_command;
    string_by_utc(&utcTimestamp);

    string_by_t SignatureNonce = string_null_command;
    string_by_id(&SignatureNonce);
    char *tmpTopicFullName;
    TopicFullNameFormat(&tmpTopicFullName, aliConfig->ProductKey.valuestring, DeviceName, "user/get");

    list_t list[] = {
        list_string_command("Action", "Pub"),
        list_string_command("ProductKey", aliConfig->ProductKey.valuestring),
        list_string_command("MessageContent", tmpContentBase64),
        list_string_command("TopicFullName", tmpTopicFullName),
        list_string_command("Format", aliConfig->Format.valuestring),
        list_string_command("Version", aliConfig->Version.valuestring),
        list_string_command("AccessKeyId", aliConfig->AccessKeyId.valuestring),
        list_string_command("SignatureMethod", aliConfig->SignatureMethod.valuestring),
        list_string_command("Timestamp", utcTimestamp.valuestring),
        list_string_command("SignatureVersion", aliConfig->SignatureVersion.valuestring),
        list_string_command("SignatureNonce", SignatureNonce.valuestring),
        list_string_command("RegionId", aliConfig->RegionId.valuestring)};
    list_sort(list, list_count(list));
    char *SignatureString;
    SignatureFormat(&SignatureString, list, list_count(list), aliConfig->AccessKeySecret.valuestring);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, list, list_count(list), SignatureString);
    AliyunHttpsCallback callback;
    AliyunHttpsGET(tmpUrlString, &callback);
    if (result)
    {
        string_create(jsonstring, callback.memory, callback.size);
    }
    https_result_delete(result);
    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);
    global_hooks.deallocate(tmpContentBase64);
    string_delete(utcTimestamp);
    string_delete(SignatureNonce);
    global_hooks.deallocate(tmpTopicFullName);
    return *jsonstring;
}

// 通过自定义Topic向指定设备发布消息
string_by_t HTTPSAliyunRegisterDevice(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName)
{

    https_result *result;
    https_result_create(&result);
    string_by_t utcTimestamp = string_null_command;
    string_by_utc(&utcTimestamp);

    string_by_t SignatureNonce = string_null_command;
    string_by_id(&SignatureNonce);

    struct _list_t list[] = {
        list_string_command("Action", "RegisterDevice"),
        list_string_command("ProductKey", aliConfig->ProductKey.valuestring),
        list_string_command("DeviceName", DeviceName),
        list_string_command("Format", aliConfig->Format.valuestring),
        list_string_command("Version", aliConfig->Version.valuestring),
        list_string_command("AccessKeyId", aliConfig->AccessKeyId.valuestring),
        list_string_command("SignatureMethod", aliConfig->SignatureMethod.valuestring),
        list_string_command("Timestamp", utcTimestamp.valuestring),
        list_string_command("SignatureVersion", aliConfig->SignatureVersion.valuestring),
        list_string_command("SignatureNonce", SignatureNonce.valuestring),
        list_string_command("RegionId", aliConfig->RegionId.valuestring)};
    list_sort(list, list_count(list));
    char *SignatureString;
    SignatureFormat(&SignatureString, list, list_count(list), aliConfig->AccessKeySecret.valuestring);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, list, list_count(list), SignatureString);
    AliyunHttpsCallback callback;
    AliyunHttpsGET(tmpUrlString, &callback);
    if (result)
    {
        string_create(jsonstring, callback.memory, callback.size);
    }

    https_result_delete(result);
    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);
    string_delete(utcTimestamp);
    string_delete(SignatureNonce);
    return *jsonstring;
}

string_by_t HTTPSAliyunGetDeviceStatus(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName)
{

    https_result *result;
    https_result_create(&result);
    string_by_t utcTimestamp = string_null_command;
    string_by_utc(&utcTimestamp);
    string_by_t SignatureNonce = string_null_command;
    string_by_id(&SignatureNonce);
    list_t list[] = {
        list_string_command("Action", "GetDeviceStatus"),
        list_string_command("ProductKey", aliConfig->ProductKey.valuestring),
        list_string_command("DeviceName", DeviceName),
        list_string_command("Format", aliConfig->Format.valuestring),
        list_string_command("Version", aliConfig->Version.valuestring),
        list_string_command("AccessKeyId", aliConfig->AccessKeyId.valuestring),
        list_string_command("SignatureMethod", aliConfig->SignatureMethod.valuestring),
        list_string_command("Timestamp", utcTimestamp.valuestring),
        list_string_command("SignatureVersion", aliConfig->SignatureVersion.valuestring),
        list_string_command("SignatureNonce", SignatureNonce.valuestring),
        list_string_command("RegionId", aliConfig->RegionId.valuestring)};
    list_sort(list, list_count(list));
    char *SignatureString;
    SignatureFormat(&SignatureString, list, list_count(list), aliConfig->AccessKeySecret.valuestring);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, list, list_count(list), SignatureString);
    AliyunHttpsCallback callback;
    AliyunHttpsGET(tmpUrlString, &callback);
    if (result)
    {
        string_create(jsonstring, callback.memory, callback.size);
    }
    https_result_delete(result);
    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);

    string_delete(utcTimestamp);
    string_delete(SignatureNonce);
    return *jsonstring;
}
