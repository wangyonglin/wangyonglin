#include <WechatPayment.h>
#include <string_by_this.h>
#include <cJSON.h>
#include <Encrypt/SHA256WithRSA.h>

#include <string_by_hex.h>
#include <string_by_id.h>

#include <string_by_log.h>

#include <string_by_timestamp.h>
#include <string_by_inject.h>
#include <byte_by_this.h>

WechatPayment *WechatPaymentCreate(config_t *config, WechatPayment **payment, const char *section)
{
    if (config && config->inject)
    {
        if (object_create((void **)payment, sizeof(WechatPayment)))
        {
            inject_command_t commands[] = {
                inject_string_command("appid", offsetof(WechatPayment, appid)),
                inject_string_command("secret", offsetof(WechatPayment, secret)),
                inject_string_command("mchid", offsetof(WechatPayment, mchid)),
                inject_string_command("serial_no", offsetof(WechatPayment, serial_no)),
                inject_string_command("apiclient_key", offsetof(WechatPayment, apiclient_key)),
                inject_string_command("notify_url", offsetof(WechatPayment, notify_url)),
                inject_null_command};
            inject_build(config->inject, commands, *payment, "WECHAT_PAYMENT");
            return (*payment);
        }
    }
    return NULL;
}
void WechatPaymentDelete(WechatPayment *payment)
{
    string_delete(payment->appid);
    string_delete(payment->secret);
    string_delete(payment->mchid);
    string_delete(payment->serial_no);
    string_delete(payment->apiclient_key);
    string_delete(payment->notify_url);
    object_delete(payment);
}

void WechatPaymentPurchaseAuthorization(WechatPayment *payment, char *OpenId,integer_by_t total, string_by_t url, string_by_t *body, string_by_t *authorization)
{

    string_by_t id = string_null_command;
    string_by_id(&id);

    string_by_t timestamp = string_null_command;
    string_by_timestamp(&timestamp);

    string_by_t nonce_str = string_null_command;
    string_by_hex(&nonce_str, 32);

    // Body参数配置
    cJSON *root = cJSON_CreateObject();
    cJSON *amount;
    cJSON *payer;
    if (root)
    {
        cJSON_AddStringToObject(root, "appid", payment->appid.valuestring);
        cJSON_AddStringToObject(root, "mchid", payment->mchid.valuestring);
        cJSON_AddStringToObject(root, "description", timestamp.valuestring);
        cJSON_AddStringToObject(root, "out_trade_no", id.valuestring);
        cJSON_AddStringToObject(root, "notify_url", "https://www.weixin.qq.com/wxpay/pay.php");
        cJSON_AddItemToObject(root, "amount", amount = cJSON_CreateObject());
        cJSON_AddStringToObject(amount, "currency", "CNY");
        cJSON_AddNumberToObject(amount, "total", total);
        cJSON_AddItemToObject(root, "payer", payer = cJSON_CreateObject());
        cJSON_AddStringToObject(payer, "openid", OpenId);
        char *jsonString = cJSON_PrintUnformatted(root);
        if (jsonString)
        {
            string_create(body, jsonString, strlen(jsonString));
            free(jsonString);
        }
        cJSON_free(root);
    }

    size_t tmpStringSize = 0;
    char *tmpString;
    byte_create(&tmpString, NULL, 1024);

    strcat(tmpString, "POST");
    strcat(tmpString, "\n");
    strcat(tmpString, url.valuestring);
    strcat(tmpString, "\n");
    strcat(tmpString, timestamp.valuestring);
    strcat(tmpString, "\n");
    strcat(tmpString, nonce_str.valuestring);
    strcat(tmpString, "\n");
    strcat(tmpString, body->valuestring);
    strcat(tmpString, "\n");
    fprintf(stdout, "[apiclient_key=>%s]\n", payment->apiclient_key.valuestring);
    char *signatureString;
    if (Sha256WithRSASignatureEx(payment->apiclient_key, tmpString, strlen(tmpString), &signatureString))
    {
        size_t authStringSize = 106;
        authStringSize += payment->mchid.valuelength;
        authStringSize += nonce_str.valuelength;
        authStringSize += strlen(signatureString);
        authStringSize += timestamp.valuelength;
        authStringSize += payment->serial_no.valuelength;
        char *authString = NULL;
        byte_create(&authString, NULL, authStringSize);
        sprintf(authString, "WECHATPAY2-SHA256-RSA2048 mchid=\"%s\",nonce_str=\"%s\",signature=\"%s\",timestamp=\"%s\",serial_no=\"%s\"",
                payment->mchid.valuestring,
                nonce_str.valuestring,
                signatureString,
                timestamp.valuestring,
                payment->serial_no.valuestring);
        string_create(authorization, authString, strlen(authString));
        byte_delete(signatureString);
        byte_delete(authString);

        string_delete(id);
        string_delete(timestamp);
        string_delete(nonce_str);
    }
    byte_delete(tmpString);
}
