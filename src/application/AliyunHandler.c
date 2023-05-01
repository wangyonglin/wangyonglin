
#include <SnowFlake.h>
#include <ArrayList.h>
#include <SnowFlake.h>
#include <base64.h>
#include <evhttp.h>
#include <HMAC_SHA1.h>
#include <curl/curl.h>
#include <AliyunHandler.h>
#include <AliyunUtils.h>

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
            StringexBase64Encrypt(&MessageContentBase64, MessageContentString, strlen(MessageContentString), false);
            global_hooks.deallocate(MessageContentString);

            Stringex Timestamp;
            StringexTimeUTC(&Timestamp);

            Stringex SignatureNonce;
            StringexId(&SignatureNonce);
            char *tmpTopicFullName;
            TopicFullNameFormat(&tmpTopicFullName, aliConfig->ProductKey.valuestring, DeviceName->valuestring, "user/get");

            ArrayList arrayList[] = {
                ARRAYLIST_STRING_COMMAND("Action", "Pub"),
                ARRAYLIST_STRING_COMMAND("ProductKey", aliConfig->ProductKey.valuestring),
                ARRAYLIST_STRING_COMMAND("MessageContent", MessageContentBase64.valuestring),
                ARRAYLIST_STRING_COMMAND("TopicFullName", tmpTopicFullName),
                ARRAYLIST_STRING_COMMAND("Format", aliConfig->Format.valuestring),
                ARRAYLIST_STRING_COMMAND("Version", aliConfig->Version.valuestring),
                ARRAYLIST_STRING_COMMAND("AccessKeyId", aliConfig->AccessKeyId.valuestring),
                ARRAYLIST_STRING_COMMAND("SignatureMethod", aliConfig->SignatureMethod.valuestring),
                ARRAYLIST_STRING_COMMAND("Timestamp", Timestamp.valuestring),
                ARRAYLIST_STRING_COMMAND("SignatureVersion", aliConfig->SignatureVersion.valuestring),
                ARRAYLIST_STRING_COMMAND("SignatureNonce", SignatureNonce.valuestring),
                ARRAYLIST_STRING_COMMAND("RegionId", aliConfig->RegionId.valuestring)};
            ArrayListSort(arrayList, ArrayListCount(arrayList));
            char *SignatureString;
            SignatureFormat(&SignatureString, arrayList, ArrayListCount(arrayList), aliConfig->AccessKeySecret.valuestring);
            char *tmpUrlString;
            URLFormat(&tmpUrlString, arrayList, ArrayListCount(arrayList), SignatureString);
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

    ArrayList arrayList[] = {
        ARRAYLIST_STRING_COMMAND("Action", "RegisterDevice"),
        ARRAYLIST_STRING_COMMAND("ProductKey", aliConfig->ProductKey.valuestring),
        ARRAYLIST_STRING_COMMAND("DeviceName", DeviceName),
        ARRAYLIST_STRING_COMMAND("Format", aliConfig->Format.valuestring),
        ARRAYLIST_STRING_COMMAND("Version", aliConfig->Version.valuestring),
        ARRAYLIST_STRING_COMMAND("AccessKeyId", aliConfig->AccessKeyId.valuestring),
        ARRAYLIST_STRING_COMMAND("SignatureMethod", aliConfig->SignatureMethod.valuestring),
        ARRAYLIST_STRING_COMMAND("Timestamp", utcTimestamp.valuestring),
        ARRAYLIST_STRING_COMMAND("SignatureVersion", aliConfig->SignatureVersion.valuestring),
        ARRAYLIST_STRING_COMMAND("SignatureNonce", SignatureNonce.valuestring),
        ARRAYLIST_STRING_COMMAND("RegionId", aliConfig->RegionId.valuestring)};
    ArrayListSort(arrayList, ArrayListCount(arrayList));
    char *SignatureString;
    SignatureFormat(&SignatureString, arrayList, ArrayListCount(arrayList), aliConfig->AccessKeySecret.valuestring);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, arrayList, ArrayListCount(arrayList), SignatureString);
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
      ArrayList arrayList[] = {
        ARRAYLIST_STRING_COMMAND("Action", "GetDeviceStatus"),
        ARRAYLIST_STRING_COMMAND("ProductKey", aliConfig->ProductKey.valuestring),
        ARRAYLIST_STRING_COMMAND("DeviceName", DeviceName),
        ARRAYLIST_STRING_COMMAND("Format", aliConfig->Format.valuestring),
        ARRAYLIST_STRING_COMMAND("Version", aliConfig->Version.valuestring),
        ARRAYLIST_STRING_COMMAND("AccessKeyId", aliConfig->AccessKeyId.valuestring),
        ARRAYLIST_STRING_COMMAND("SignatureMethod", aliConfig->SignatureMethod.valuestring),
        ARRAYLIST_STRING_COMMAND("Timestamp", utcTimestamp.valuestring),
        ARRAYLIST_STRING_COMMAND("SignatureVersion", aliConfig->SignatureVersion.valuestring),
        ARRAYLIST_STRING_COMMAND("SignatureNonce", SignatureNonce.valuestring),
        ARRAYLIST_STRING_COMMAND("RegionId", aliConfig->RegionId.valuestring)};
    ArrayListSort(arrayList, ArrayListCount(arrayList));
    char *SignatureString;
    SignatureFormat(&SignatureString, arrayList, ArrayListCount(arrayList), aliConfig->AccessKeySecret.valuestring);
    char *tmpUrlString;
    URLFormat(&tmpUrlString, arrayList, ArrayListCount(arrayList), SignatureString);
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
