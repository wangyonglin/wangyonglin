#include <https_get.h>



long https_get(https_result *result, const char *url)
{
    if (!result && !url)
    {
        return 400;
    }
    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
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
        }
        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                    curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

    return res;
}