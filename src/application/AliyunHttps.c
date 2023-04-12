#include <AliyunHttps.h>

static size_t
AliyunWriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    AliyunHttpsCallback *callback = (AliyunHttpsCallback *)userp;

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

long AliyunHttpsGET(char *url, AliyunHttpsCallback *callback)
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
        // list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
        // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // 启用时会汇报所有的信息
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, AliyunWriteMemoryCallback);
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