#include <Tiktok/TiktokConfig.h>

size_t TiktokCallbackWriteMemory(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    TiktokCallbackResponse *tiktokCallbackResponse = (TiktokCallbackResponse *)userp;

    // 注意这里根据每次被调用获得的数据重新动态分配缓存区的大小
    char *ptr = realloc(tiktokCallbackResponse->memory, tiktokCallbackResponse->size + realsize + 1);

    if (ptr == NULL)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    tiktokCallbackResponse->memory = ptr;
    memset(&(tiktokCallbackResponse->memory[tiktokCallbackResponse->size]), 0x00, tiktokCallbackResponse->size + realsize + 1);
    memcpy(&(tiktokCallbackResponse->memory[tiktokCallbackResponse->size]), contents, realsize);
    tiktokCallbackResponse->size += realsize;
    tiktokCallbackResponse->memory[tiktokCallbackResponse->size] = 0;

    return realsize;
}

TiktokConfig *TiktokConfigCreate(TiktokConfig **tiktokConfig, MainConfig *mainConfig, const char *section)
{
    if (!mainConfig)
    {
        return NULL;
    }
    if (!section)
    {
        return NULL;
    }
    if (ObjectCreate((void **)tiktokConfig, sizeof(TiktokConfig)))
    {
        (*tiktokConfig)->appid = Stringex_null;
        (*tiktokConfig)->secret = Stringex_null;
        (*tiktokConfig)->public_key = Stringex_null;
        InjectCommand injectCommand[] = {
            INJECT_COMMAND_STRING("appid", offsetof(TiktokConfig, appid)),
            INJECT_COMMAND_STRING("secret", offsetof(TiktokConfig, secret)),
            INJECT_COMMAND_FILE("public_key", offsetof(TiktokConfig, public_key)),
            INJECT_COMMAND_NULL};
        InjectCommandInit(mainConfig->injectObject, injectCommand, *tiktokConfig, "TIKTOK");
        return (*tiktokConfig);
    }
    return NULL;
}
void TiktokConfigDelete(TiktokConfig *tiktokConfig)
{
    if (tiktokConfig)
    {
        StringexDelete(tiktokConfig->appid);
        StringexDelete(tiktokConfig->secret);
        StringexDelete(tiktokConfig->public_key);
        ObjectDelete(tiktokConfig);
    }
}

Boolean TiktokPostToken(char *urlvalue, Stringex access_token, Stringex auth, Stringex body, TiktokCallbackResponse *tiktokCallbackResponse)
{
    Boolean err = Boolean_false;
    tiktokCallbackResponse->memory = malloc(1); /* will be grown as needed by the realloc above */
    tiktokCallbackResponse->size = 0;           /* no data at this point */

    CURL *curl;
    CURLcode retcode;

    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if (curl)
    {
        list = curl_slist_append(list, "Content-Type: application/json");
        list = curl_slist_append(list, "Accept: application/json");
        if (Stringex_IsString(access_token) && Stringex_IsEmpty(access_token))
        {
            char tmpstring[16 + access_token.valuelength];
            bzero(tmpstring, sizeof(tmpstring));
            strcat(tmpstring, "access-token:");
            strcat(tmpstring, access_token.valuestring);
            list = curl_slist_append(list, tmpstring);
        }

        if (Stringex_IsString(auth) && Stringex_IsEmpty(auth))
        {
            char tmpstring[16 + auth.valuelength];
            bzero(tmpstring, sizeof(tmpstring));
            strcat(tmpstring, "Byte-Authorization:");
            strcat(tmpstring, auth.valuestring);
            list = curl_slist_append(list, tmpstring);
        }
        // list = curl_slist_append(list, "Content-Type: application/json; charset=utf-8");
        // list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // 启用时会汇报所有的信息
        curl_easy_setopt(curl, CURLOPT_URL, urlvalue);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TiktokCallbackWriteMemory);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tiktokCallbackResponse);

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        if (Stringex_IsString(body) && Stringex_IsEmpty(body))
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.valuestring);
        }
    }

    retcode = curl_easy_perform(curl);
    // fprintf(stdout,"[%s=> %d]",url.valuestring,retcode);
#ifdef SKIP_PEER_VERIFICATION

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
    /* check for errors */
    if (retcode != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(retcode));
    }
    else
    {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */

        //  printf("%lu bytes retrieved\n", (unsigned long)callback->size);
    }

    curl_slist_free_all(list);
    /* always cleanup */
    curl_easy_cleanup(curl);
    return err;
}

Boolean TiktokPost(char *urlvalue, Stringex auth, Stringex body, TiktokCallbackResponse *tiktokCallbackResponse)
{
    Boolean err = Boolean_false;
    tiktokCallbackResponse->memory = malloc(1); /* will be grown as needed by the realloc above */
    tiktokCallbackResponse->size = 0;           /* no data at this point */

    CURL *curl;
    CURLcode retcode;

    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if (curl)
    {
        list = curl_slist_append(list, "Content-Type: application/json");
        list = curl_slist_append(list, "Accept: application/json");
        if (Stringex_IsString(auth) && Stringex_IsEmpty(auth))
        {
            char tmpstring[16 + auth.valuelength];
            bzero(tmpstring, sizeof(tmpstring));
            strcat(tmpstring, "Byte-Authorization:");
            strcat(tmpstring, auth.valuestring);
            list = curl_slist_append(list, tmpstring);
        }
        // list = curl_slist_append(list, "Content-Type: application/json; charset=utf-8");
        // list = curl_slist_append(list, "User-Agent:Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.132 Safari/537.36");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // 启用时会汇报所有的信息
        curl_easy_setopt(curl, CURLOPT_URL, urlvalue);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TiktokCallbackWriteMemory);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tiktokCallbackResponse);

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

        if (Stringex_IsString(body) && Stringex_IsEmpty(body))
        {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.valuestring);
        }
    }

    retcode = curl_easy_perform(curl);
    // fprintf(stdout,"[%s=> %d]",url.valuestring,retcode);
#ifdef SKIP_PEER_VERIFICATION

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif
    /* check for errors */
    if (retcode != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(retcode));
    }
    else
    {
        /*
         * Now, our chunk.memory points to a memory block that is chunk.size
         * bytes big and contains the remote file.
         *
         * Do something nice with it!
         */

        //  printf("%lu bytes retrieved\n", (unsigned long)callback->size);
    }

    curl_slist_free_all(list);
    /* always cleanup */
    curl_easy_cleanup(curl);
    return err;
}