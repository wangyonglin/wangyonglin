#include <wangyonglin/core.h>
#include <wangyonglin/config.h>
/*
typedef struct
{
    char appid[32];
    char mchid[32];
    char serial_no[32];
    char APIv3_key[32];
    char *nonceStr;
    char *timestamp;
    char *plaintext;
    int plainlenth;
    char cipherfiles[128];
    char *signature;
    char *token;
} wechat_config_t;
void buildSignature(char **signature, char *key, const char *method, const char *url, char *timestamp, char *nonceStr, char *body)
{
    BUF_MEM *bptr;
    BIO *mem = BIO_new(BIO_s_mem());
    BIO_puts(mem, method);
    BIO_puts(mem, "\n");
    BIO_puts(mem, url);
    BIO_puts(mem, "\n");
    BIO_puts(mem, timestamp);
    BIO_puts(mem, "\n");
    BIO_puts(mem, nonceStr);
    BIO_puts(mem, "\n");
    BIO_puts(mem, body);
    BIO_puts(mem, "\n");
    BIO_get_mem_ptr(mem, &bptr);
    // BIO_set_close(mem, BIO_NOCLOSE); /* BIO_free() 不释放BUF_MEM结构 */
// char *message = SHA256WithRSA_Signature_RSAPrivateKey_file(key, bptr->data, bptr->length);
// printf("\tplaintext:%d\r\n%s\r\n", bptr->length, bptr->data);
// int len = strlen(message);
// printf("signature:%d\r\n%s\r\n", strlen(message), message);
// (*signature) = (char *)malloc(len + 1);
//  memset(*signature, 0x00, len + 1);
//  memcpy(*signature, message, len);
//  BIO_free(mem);
// free(message);
//}
/*
int buildToken(char **token, const char *mchid, char *nonce_str, char *timestamp, char *serial_no, char *signature)
{
    const char format[] = "WECHATPAY2-SHA256-RSA2048 mchid=\"%s\",nonce_str=\"%s\",timestamp=\"%s\",serial_no=\"%s\",signature=\"%s\"";
    int lenth = strlen(format) + strlen(mchid) + strlen(nonce_str) + strlen(timestamp) + strlen(serial_no) + strlen(signature);
    (*token) = (char *)malloc(lenth + 1);
    memset(*token, 0x00, sizeof(*signature));
    sprintf(*token, format, mchid, nonce_str, timestamp, serial_no, signature);

    // printf("\ttoken:%d\r\n%s\r\n", strlen(*token), *token);
    return 0;
}

int geturl(const char *url, char *token, char *body)
{
    CURL *curl;
    CURLcode res;

    char buffer[1024];
    bzero(buffer, sizeof(buffer));
    curl_global_init(CURL_GLOBAL_DEFAULT);

    curl = curl_easy_init();
    if (curl)
    {

        sprintf(buffer, "Authorization:%s", token);
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, buffer);
        chunk = curl_slist_append(chunk, "Content-Type:application/json");
        chunk = curl_slist_append(chunk, "User-Agent:https://zh.wikipedia.org/wiki/User_agent");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        /* Now specify the POST data */
/*
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
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

return 0;
}
*/
int main()
{
    /*
    wechat_config_t *config = (wechat_config_t *)malloc(sizeof(wechat_config_t));
    memset(config, 0x00, sizeof(config));
    int ret = timestamp(&config->timestamp);
    printf("\ttimestamp:%s\r\n", config->timestamp);
    noncehex(&config->nonceStr, 128);
    printf("\tnonceStr:%s\r\n", config->nonceStr);
    strcpy(config->appid, strdup(""));
    printf("\tappid:%s\r\n", config->appid);
    strcpy(config->mchid, strdup(""));
    printf("\tmchid:%s\r\n", config->mchid);
    strcpy(config->serial_no, strdup(""));
    printf("\tserial_no:%s\r\n", config->serial_no);
    strcpy(config->cipherfiles, strdup("./apiclient_key.pem"));
    printf("\tcipherfiles:%s\r\n", config->cipherfiles);

    cJSON *root = cJSON_CreateObject();
    cJSON *amount = NULL;
    cJSON *payer = NULL;
    cJSON_AddStringToObject(root, "appid", config->appid);
    cJSON_AddStringToObject(root, "mchid", config->mchid);
    cJSON_AddStringToObject(root, "description", "rect");
    cJSON_AddStringToObject(root, "out_trade_no", "123456789");
    cJSON_AddStringToObject(root, "notify_url", "https://pay.wangyonglin.com/wechat/notify_url");
    cJSON_AddItemToObject(root, "amount", amount = cJSON_CreateObject());
    cJSON_AddItemToObject(amount, "total", cJSON_CreateNumber(100));
    cJSON_AddItemToObject(amount, "currency", cJSON_CreateString("CNY"));
    cJSON_AddItemToObject(root, "payer", payer = cJSON_CreateObject());
    cJSON_AddItemToObject(payer, "openid", cJSON_CreateString("123456"));
    char *out = cJSON_Print(root);
    buildSignature(&config->signature, config->cipherfiles, "POST", "/v3/pay/transactions/jsapi", config->timestamp, config->nonceStr, out);
    printf("\tsignature:%s \t\n", config->signature);
    buildToken(&config->token, config->mchid, config->nonceStr, config->timestamp, config->serial_no, config->signature);
    printf("\ttoken:%s \t\n", config->token);

   // geturl("https://api.mch.weixin.qq.com/v3/pay/transactions/jsapi", config->token,out);
    // free(out);
    //  cJSON_Delete(root);

    /*
    https_client_t client;
    int retd = 0;
    retd = https_client(&client, "https://api.mch.weixin.qq.com/v3/certificates", -1);
    fprintf(stderr, "\r\n\t%s\r\n", client.reps.reason);
    fprintf(stderr, "\r\n\t%d\r\n", client.reps.error_code);
    fprintf(stderr, "\r\n\t%d %s\r\n", client.reps.lenght, client.reps.body);

    printf("\t\n");

    return retd;*/

    config_t *config;
    config_init(&config, "/home/wangyonglin/github/wangyonglin/conf/wangyonglin.conf");

    // pid(config);

    log_init(config);
    log_echo(config,LOG_INFO,"wangyonglin");
    log_free(config);
    config_free(config);
    return 0;
}
