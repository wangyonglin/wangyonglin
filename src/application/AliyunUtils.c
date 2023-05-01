#include <AliyunUtils.h>

#include <URL.h>
#include <HMAC_SHA1.h>
#include <ArrayList.h>

#include <SnowFlake.h>
#include <base64.h>
#include <Stringex.h>

char *TopicFullNameFormat(char **outstring, char *ProductKey, char *DeviceName, char *TopicFullName)
{

    size_t tmpsize = strlen(ProductKey) + strlen(DeviceName) + strlen(TopicFullName) + 8;
    char tmpstring[tmpsize];
    memset(tmpstring, 0x00, sizeof(tmpstring));
    strcat(tmpstring, "/");
    strcat(tmpstring, ProductKey);
    strcat(tmpstring, "/");
    strcat(tmpstring, DeviceName);
    strcat(tmpstring, "/");
    strcat(tmpstring, TopicFullName);
    StringCreate(outstring, tmpstring, strlen(tmpstring));
    return *outstring;
}

// char *SignatureNonceFormat(char **outstring)
// {

//     char tmpstring[12];
//     memset(tmpstring, 0x00, sizeof(tmpstring));
//     int64_t id = SnowFlake_IdGenerator();
//     sprintf(tmpstring, "%ld", id);
//     byte_create(outstring, tmpstring, strlen(tmpstring));
//     return *outstring;
// }

// char *TimestampFormat(char **outstring)
// {
//     char tmpstring[80];
//     memset(tmpstring, 0x00, sizeof(tmpstring));
//     UtcTime result;
//     time_t rawtime;
//     time(&rawtime);
//     result = unix32_to_UTC(rawtime);
//     sprintf(tmpstring, "%04d-%02d-%02dT%02d:%02d:%02dZ", result.year, result.month, result.day,
//             result.hour, result.minute, result.second); // 以年月日_时分秒的形式表示当前时间
//     byte_create(outstring, tmpstring, strlen(tmpstring));
//     return *outstring;
// }

// char *ContentBase64(char **outstring, char *MessageContentText, size_t MessageContentSize)
// {
//     char *tmpstring = base64_encrypt(MessageContentText, MessageContentSize, true);
//     byte_create(outstring, tmpstring, strlen(tmpstring));
//     free(tmpstring);
//     return (*outstring);
// }

char *SignatureFormat(char **Signature, ArrayList arrayList[], size_t count, char *AccessKeySecret)
{
    size_t i = 0;

    char tmpstring[1024];
    memset(tmpstring, 0x00, sizeof(1024));
    strcat(tmpstring, "GET&%2F&");
    while (arrayList[i].keystring)
    {
        if (arrayList[i].valtype == OBJECT_TYPE_STRING)
        {
            strcat(tmpstring, arrayList[i].keystring);
            strcat(tmpstring, "%3D");
            strcat(tmpstring, arrayList[i].valstring);
            if (i < count - 1)
                strcat(tmpstring, "%26");
        }
        i++;
    }

    size_t resmax = 1024;
    size_t tmpSignatureLengtg = 0;
    char *tmpSignature = malloc(sizeof(char) * 1024);

    memset(tmpSignature, 0x00, sizeof(char) * 1024);
    memcpy(tmpSignature, tmpstring, strlen(tmpstring));
    tmpSignatureLengtg = strlen(tmpSignature);
    ReplaceFormat(tmpSignature, tmpSignatureLengtg, '=', "%253D");
    tmpSignatureLengtg = strlen(tmpSignature);
    ReplaceFormat(tmpSignature, tmpSignatureLengtg, '/', "%252F");
    tmpSignatureLengtg = strlen(tmpSignature);
    ReplaceFormat(tmpSignature, tmpSignatureLengtg, ':', "%253A");
    uint8_t *outstring;
    HmacSha1_Base64(AccessKeySecret, strlen(AccessKeySecret), tmpSignature, strlen(tmpSignature), &outstring, resmax);
    StringDelete(tmpSignature);
    StringCreate(Signature, outstring, strlen(outstring));
    return *Signature;
}

char *URLFormat(char **formerString, ArrayList arrayList[], size_t count, char *SignatureString)
{
    size_t signatureStringLengtg = strlen(SignatureString);
    size_t tempStringLengtg = 0;

    StringCreate(formerString, NULL, 1024);
    strcat(*formerString, "https://iot.cn-shanghai.aliyuncs.com?");
    char *tempString;
    StringCreate(&tempString, NULL, 1024);
    char *tempSignatureString;

    StringCreate(&tempSignatureString, SignatureString, signatureStringLengtg);
    ReplaceFormat(tempSignatureString, signatureStringLengtg, '+', "%2B");
    size_t i = 0;
    while (arrayList[i].keystring)
    {
        if (arrayList[i].valtype == OBJECT_TYPE_STRING)
        {
            strcat(tempString, arrayList[i].keystring);
            strcat(tempString, "=");
            strcat(tempString, arrayList[i].valstring);
            strcat(tempString, "&");
        }
        i++;
    }
    strcat(tempString, "Signature=");
    strcat(tempString, tempSignatureString);
    tempStringLengtg = strlen(SignatureString);
    ReplaceFormat(tempString, tempStringLengtg, '/', "%2F");
    tempStringLengtg = strlen(SignatureString);
    ReplaceFormat(tempString, tempStringLengtg, '-', "%2D");
    tempStringLengtg = strlen(SignatureString);
    ReplaceFormat(tempString, tempStringLengtg, ':', "%3A");
    strcat((*formerString), tempString);
    StringDelete(tempSignatureString);
    StringDelete(tempString);
    return (*formerString);
}

char *ReplaceFormat(char *formerString, size_t formerStringMax, const char findString, char *replaceString)
{

    char *tmpstring;
    StringCreate(&tmpstring, NULL, 1024);
    int formerStringSize = strlen(formerString);
    int i, j;
    char ch;
    if (formerStringSize < 0)
        return NULL;
    for (i = 0, j = 0; i < formerStringSize; i++)
    {
        if (formerString[i] == findString)
        {
            for (size_t x = 0; x < strlen(replaceString); x++)
            {
                tmpstring[j++] = replaceString[x];
            }
        }
        else
        {
            tmpstring[j++] = formerString[i];
        }
    }

    if (formerString)
    {
        memset(formerString, 0x00, formerStringMax);
        memcpy(formerString, tmpstring, strlen(tmpstring));
    }
    StringDelete(tmpstring);
    return formerString;
}