
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
#include <StringexBase64.h>
// // // 通过自定义Topic向指定设备发布消息
boolean_by_t HTTPSAliyunPub(cJSON **RetCallback, AliyunConfig *aliConfig, cJSON *DeviceName, cJSON *MessageContentText)
{
    if (aliConfig && DeviceName && MessageContentText)
    {
        char *MessageContentString = cJSON_PrintUnformatted(MessageContentText);
        if (MessageContentString)
        {
            Stringex MessageContentBase64;
            StringexBase64Encrypt(&MessageContentBase64, MessageContentString, strlen(MessageContentString),false);
            global_hooks.deallocate(MessageContentString);

            Stringex Timestamp;
            StringexTimeUTC(&Timestamp);

            Stringex SignatureNonce;
            StringexId(&SignatureNonce);
            char *tmpTopicFullName;
            TopicFullNameFormat(&tmpTopicFullName, aliConfig->ProductKey.valuestring, DeviceName->valuestring, "user/get");

            list_t list[] = {
                list_string_command("Action", "Pub"),
                list_string_command("ProductKey", aliConfig->ProductKey.valuestring),
                list_string_command("MessageContent", MessageContentBase64.valuestring),
                list_string_command("TopicFullName", tmpTopicFullName),
                list_string_command("Format", aliConfig->Format.valuestring),
                list_string_command("Version", aliConfig->Version.valuestring),
                list_string_command("AccessKeyId", aliConfig->AccessKeyId.valuestring),
                list_string_command("SignatureMethod", aliConfig->SignatureMethod.valuestring),
                list_string_command("Timestamp", Timestamp.valuestring),
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
            if (callback.memory)
            {
                *RetCallback = cJSON_Parse(callback.memory);
                global_hooks.deallocate(callback.memory);
            }

            global_hooks.deallocate(tmpUrlString);
            global_hooks.deallocate(SignatureString);
            StringexDelete(MessageContentBase64);
            StringexDelete(Timestamp);
            StringexDelete(SignatureNonce);
            global_hooks.deallocate(tmpTopicFullName);
        }
    }
    return negative;
}

// 通过自定义Topic向指定设备发布消息
boolean_by_t HTTPSAliyunRegisterDevice(cJSON **RetCallback, AliyunConfig *aliConfig, char *DeviceName)
{
    string_by_t utcTimestamp = string_null_command;
    string_by_utc(&utcTimestamp);

    string_by_t SignatureNonce = string_null_command;
    string_by_id(&SignatureNonce);

    list_t list[] = {
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
    if (callback.memory)
    {
        *RetCallback = cJSON_Parse(callback.memory);
        global_hooks.deallocate(callback.memory);
    }

    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);
    string_delete(utcTimestamp);
    string_delete(SignatureNonce);
    return negative;
}

boolean_by_t HTTPSAliyunGetDeviceStatus(cJSON **RetCallback, AliyunConfig *aliConfig, char *DeviceName)
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
        *RetCallback = cJSON_Parse(callback.memory);
        global_hooks.deallocate(callback.memory);
    }
    https_result_delete(result);
    global_hooks.deallocate(tmpUrlString);
    global_hooks.deallocate(SignatureString);

    string_delete(utcTimestamp);
    string_delete(SignatureNonce);
    return negative;
}
