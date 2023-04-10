#include <https_post.h>

long https_post(https_result *result, const char *url, char *postdata, size_t postdatasize)
{
    if (!result && !url)
    {
        return 400;
    }
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if (curl)
    {
        list = curl_slist_append(list, "Accept: application/json");
        list = curl_slist_append(list, "Content-Type: application/json; charset=utf-8");
        list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_HEADER, 0L);         // 启用时会将头文件的信息作为数据流输
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // 允许重定向
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        // libcur的相关POST配置项
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postdatasize);
#ifdef SKIP_PEER_VERIFICATION

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        long res_code = 0;
        res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);

        if ((res == CURLE_OK) && (res_code == 200 || res_code == 201))
        {
            return res;
        }
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        curl_slist_free_all(list);
        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return res;
}