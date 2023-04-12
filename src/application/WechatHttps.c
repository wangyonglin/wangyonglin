#include <WechatHttps.h>
#include <WechatPayment.h>
#include <curl/curl.h>

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    WechatHttpsCallback *callback = (WechatHttpsCallback *)userp;

    // 注意这里根据每次被调用获得的数据重新动态分配缓存区的大小
    char *ptr = realloc(callback->memory, callback->size + realsize + 1);

    if (ptr == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    callback->memory = ptr;
    memset(&(callback->memory[callback->size]), 0x00, callback->size + realsize + 1);
    memcpy(&(callback->memory[callback->size]), contents, realsize);
    callback->size += realsize;
    callback->memory[callback->size] = 0;

    return realsize;
}

long WechatHttpsPost(WechatPayment *payment,string_by_t body, string_by_t authorization, string_by_t url, WechatHttpsCallback *callback)
{
    callback->memory = malloc(1); /* will be grown as needed by the realloc above */
    callback->size = 0;           /* no data at this point */
    char tmpString[1024] = {0};
    CURL *curl;
    char authorizationString[16 + authorization.valuelength];
    bzero(authorizationString, sizeof(authorizationString));
    strcat(authorizationString, "Authorization:");
    strcat(authorizationString, authorization.valuestring);

    char urlString[32 + url.valuelength];
    bzero(urlString, sizeof(urlString));
    strcat(urlString, "https://api.mch.weixin.qq.com");
    strcat(urlString, url.valuestring);
    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if (curl)
    {
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, authorizationString);
        list = curl_slist_append(list, "Content-Type: application/json; charset=utf-8");
        list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
       // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // 启用时会汇报所有的信息
        curl_easy_setopt(curl, CURLOPT_URL, urlString);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, callback);

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.valuestring);
        callback->code = curl_easy_perform(curl);
#ifdef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        /* check for errors */
        if (callback->code != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(callback->code));
        }
        else
        {
            /*
             * Now, our chunk.memory points to a memory block that is chunk.size
             * bytes big and contains the remote file.
             *
             * Do something nice with it!
             */

            printf("%lu bytes retrieved\n", (unsigned long)callback->size);
        }

        curl_slist_free_all(list);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return callback->code;
}

// long WechatHttpsGet(WechatPayment *payment,string_by_t url)
// {
//     char jsonString[1024] = {0};
//     char authorizationString[16 + payment->authorization.datasize];
//     bzero(authorizationString, sizeof(authorizationString));
//     strcat(authorizationString, "Authorization:");
//     strcat(authorizationString, payment->authorization.valuestring);

//     char urlString[32 + url.datasize];
//     bzero(urlString, sizeof(urlString));
//     strcat(urlString, "https://api.mch.weixin.qq.com");
//     strcat(urlString, url.valuestring);

//     CURL *curl;
//     curl = curl_easy_init();
//     // char dd[] = "Authorization:WECHATPAY2-SHA256-RSA2048 mchid=\"1614902012\",nonce_str=\"3C1720331E2F0A082923252200121D1E\",signature=\"Pq3GDgSKY5Lni0qsoUt5y3DHTc5ERZXngguvXueEjoRHLgsZA47zcSGDcsP/DcX689JnLiEch+fntVpYW6E0/6Fg5v7PZFf30HNOWcid/FcADSf3AsiqzMbBhsx8fheT+RBJE40lNcgPWqWKlADEQcboBkHF5SanHzolk8Oa8Ep5PSaz67J1oCOOo97UwGwa4Sflu2DoIIFs8epfCArpOsidSxGWzeRAy/1742wchNRU4CNiK8iziSzef1XSxawLTwfVmWx5SKOTzF3K9FuBZPZWi3D24mN1QBzxQv7XvdsJ0P1yChnhZ8siYA084Vz6SpqjA7x2dzybkIvu+iiXQA==\",timestamp=\"1679848395\",serial_no=\"199BD35B56B13A3CB38742029388EE12E0A13387\"";
//     struct curl_slist *list = NULL;
//     list = curl_slist_append(list, "Accept:application/json");
//     list = curl_slist_append(list, authorizationString);
//     list = curl_slist_append(list, "Content-Type:application/json;charset=utf-8");
//     list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
//     curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

//     // 初始化
//     curl_easy_setopt(curl, CURLOPT_URL, urlString);                     // 设置post address
//     curl_easy_setopt(curl, CURLOPT_TIMEOUT, 13L);                       /// 设置超时
//     curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);                // 重连超时
//     curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);                        // 启用时会汇报所有的信息
//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // 接收数据回调
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, jsonString);              // 接收数据存放
//     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);                  // 验证服务器证书有效性
//     curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2);                  // 检验证书中的主机名和你访问的主机名一致
//                                                                         // curl_easy_setopt(curl, CURLOPT_CAINFO, “/ oem / data / cacert.pem”);                  // 指定 CA 证书路径
//     CURLcode res = curl_easy_perform(curl);                             // 开启连接

//     if (res != CURLE_OK)
//         fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//     printf("result res = % d, resp is % s\r\n", res, jsonString); // 打印接收到的网页
//     curl_easy_cleanup(curl);
//     curl_slist_free_all(list);
//     return res;
// }

long WechatHttpsJscode2session(char * url, WechatHttpsCallback *callback)
{
    callback->memory = malloc(1); /* will be grown as needed by the realloc above */
    callback->size = 0;           /* no data at this point */
    CURL *curl;

    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if (curl)
    {
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, "Content-Type: application/json; charset=utf-8");
        list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
       // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // 启用时会汇报所有的信息
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, callback);  
        callback->code = curl_easy_perform(curl);
#ifdef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
        /* check for errors */
        if (callback->code != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(callback->code));
        }
        else
        {
            /*
             * Now, our chunk.memory points to a memory block that is chunk.size
             * bytes big and contains the remote file.
             *
             * Do something nice with it!
             */

            printf("%lu bytes retrieved\n", (unsigned long)callback->size);
        }

        curl_slist_free_all(list);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }
    return callback->code;
}