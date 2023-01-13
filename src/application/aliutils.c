#include <aliutils.h>
#include <wangyonglin/string.h>
#include <wangyonglin/Unix_timestamp_converter.h>
#include <URL.h>
#include <HMAC_SHA1.h>
#include <wangyonglin/listitem.h>
char *aliurls_encode(char **outputString, size_t outputStringMax, char *inputString, size_t inputStringSize);
char *alitimestamp(char **obj)
{
    strnull(obj, 80);
    UtcTime result;
    time_t rawtime;
    time(&rawtime);
    result = unix32_to_UTC(rawtime);
    sprintf((*obj), "%04d-%02d-%02dT%02d:%02d:%02dZ", result.year, result.month, result.day,
            result.hour, result.minute, result.second); // 以年月日_时分秒的形式表示当前时间
    return (*obj);
}
int aliurls_base64(char **base64TextUrl, const int resmax, const char *strtypes, char *AccessKeySecret)
{

    char *prepareSignature;
    char *argumentsUrl;
    char *base64Text;
    strnull(&prepareSignature, resmax);
    strncat(prepareSignature, strdup("GET&%2F&"), strlen("GET&%2F&"));
    aliurls_encode(&argumentsUrl, resmax, strtypes, strlen(strtypes));
    // string_rows("argumentsUrl", argumentsUrl);
    strncat(prepareSignature, argumentsUrl, strlen(argumentsUrl));
    // string_rows("prepareSignature", prepareSignature);
    HmacSha1_Base64(AccessKeySecret, strlen(AccessKeySecret), prepareSignature, strlen(prepareSignature), (uint8_t **)&base64Text, resmax);
    // string_rows("base64Text", base64Text);
    URLReplace(base64Text, resmax, '/', "%2F");
    URLReplace(base64Text, resmax, '+', "%2B");
    URLReplace(base64Text, resmax, '=', "%3D");
    strnull(base64TextUrl, resmax);
    strncpy(*base64TextUrl, base64Text, strlen(base64Text));
    strdel(base64Text);
    strdel(argumentsUrl);
    strdel(prepareSignature);
    return 0;
}

char *aliurls_encode(char **outputString, size_t outputStringMax, char *inputString, size_t inputStringSize)
{
    char *tmpString;
    char *tmpoutString;
    strnull(&tmpoutString, outputStringMax);
    strnull(&tmpString, outputStringMax);
    memcpy(tmpString, inputString, inputStringSize);
    // string_rows("tmpString", tmpString);
    URLEncode(tmpString, strlen(tmpString), tmpoutString, outputStringMax);
   // string_rows("tmpoutString", tmpoutString);
    strnull(outputString, outputStringMax);
    memcpy(*outputString, tmpoutString, strlen(tmpoutString));
    strdel(tmpString);
    strdel(tmpoutString);
    return (*outputString);
}

char *aliutls_url_listitem(char **outputString, size_t outputStringMax, listitem *list)
{

    char out[125] = {0};
    strnull(outputString, outputStringMax);
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
        strncat(*outputString, out, strlen(out));
        if (i + 1 < list->items_pos)
        {
            strncat(*outputString, "&", strlen("&"));
        }
    }
    return *outputString;
}

void aliutils_https_get(char *url)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);

#ifdef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
}