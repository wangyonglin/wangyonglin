
#include <SnowFlake.h>
#include <wangyonglin/list.h>
#include <SnowFlake.h>
#include <Encrypt/Base64.h>
#include <evhttp.h>
#include <Encrypt/HMAC_SHA1.h>
#include <curl/curl.h>
#include <AliyunHandler.h>
#include <AliyunUtils.h>
#include <wangyonglin/wangyonglin.h>
#include <AliyunConfig.h>
#include <https_get.h>
#include <string_by_timestamp.h>

// // // 通过自定义Topic向指定设备发布消息
// string_t aliapis_https_publish(localapis_t *aliapis, char *DeviceName, char *MessageContentText, size_t MessageContentSize)
// {
//     string_t jsonstring = string_null_command;
//     https_result *result;
//     https_result_create(&result);
//     char *tmpContentBase64;
//     ContentBase64(&tmpContentBase64, MessageContentText, MessageContentSize);
//     char *tmpTimestamp;
//     TimestampFormat(&tmpTimestamp);
//     char *tmpSignatureNonce;
//     SignatureNonceFormat(&tmpSignatureNonce);
//     char *tmpTopicFullName;
//     TopicFullNameFormat(&tmpTopicFullName, aliapis->ProductKey, DeviceName, "user/get");

//     struct _list_t list[] = {
//         list_string_command("Action", "Pub"),
//         list_string_command("ProductKey", aliapis->ProductKey),
//         list_string_command("MessageContent", tmpContentBase64),
//         list_string_command("TopicFullName", tmpTopicFullName),
//         list_string_command("Format", aliapis->Format),
//         list_string_command("Version", aliapis->Version),
//         list_string_command("AccessKeyId", aliapis->AccessKeyId),
//         list_string_command("SignatureMethod", aliapis->SignatureMethod),
//         list_string_command("Timestamp", tmpTimestamp),
//         list_string_command("SignatureVersion", aliapis->SignatureVersion),
//         list_string_command("SignatureNonce", tmpSignatureNonce),
//         list_string_command("RegionId", aliapis->RegionId)};
//     list_sort(list, list_count(list));
//     char *SignatureString;
//     SignatureFormat(&SignatureString, list, list_count(list), aliapis->AccessKeySecret);
//     char *tmpUrlString;
//     URLFormat(&tmpUrlString, list, list_count(list), SignatureString);
//     https_get(result, tmpUrlString);

//     if (result)
//     {
//         jsonstring = string_create(result->jsonformat, result->jsonsize);
//         string_print("[aliapis_https_register_device]", jsonstring);
//     }
//     https_result_delete(result);
//     global_hooks.deallocate(tmpUrlString);
//     global_hooks.deallocate(SignatureString);
//     global_hooks.deallocate(tmpContentBase64);
//     global_hooks.deallocate(tmpTimestamp);
//     global_hooks.deallocate(tmpSignatureNonce);
//     global_hooks.deallocate(tmpTopicFullName);
//     return jsonstring;
// }

// 通过自定义Topic向指定设备发布消息
string_by_t aliapis_https_register_device(string_by_t *jsonstring, AliyunConfig *aliConfig, char *DeviceName)
{

    https_result *result;
    https_result_create(&result);
    char *tmpTimestamp;
    TimestampFormat(&tmpTimestamp);
    char *tmpSignatureNonce;
    SignatureNonceFormat(&tmpSignatureNonce);
    struct _list_t list[] = {
        list_string_command("Action", "RegisterDevice"),
        list_string_command("ProductKey", aliConfig->ProductKey.valuestring),
        list_string_command("DeviceName", DeviceName),
        list_string_command("Format", aliConfig->Format.valuestring),
        list_string_command("Version", aliConfig->Version.valuestring),
        list_string_command("AccessKeyId", aliConfig->AccessKeyId.valuestring),
        list_string_command("SignatureMethod", aliConfig->SignatureMethod.valuestring),
        list_string_command("Timestamp", tmpTimestamp),
        list_string_command("SignatureVersion", aliConfig->SignatureVersion.valuestring),
        list_string_command("SignatureNonce", tmpSignatureNonce),
        list_string_command("RegionId", aliConfig->RegionId.valuestring)};
    list_sort(list, list_count(list));
    char *SignatureString;
    SignatureFormat(&SignatureString, list, list_count(list), aliConfig->AccessKeySecret.valuestring);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, list, list_count(list), SignatureString);
    https_get(result, tmpUrlString);
    if (result)
    {
        string_create(jsonstring, result->jsonformat, result->jsonsize);
    }
    https_result_delete(result);
    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);
    global_hooks.deallocate(tmpTimestamp);
    global_hooks.deallocate(tmpSignatureNonce);
    return *jsonstring;
}

