#include <WechatHandlerNotifyUrl.h>
#include <StringexSha256WithRsa.h>
#include <ServerResponse.h>
#include <WechatConfigCertificates.h>
const size_t KEY_LENGTH_BYTE = 32;
const size_t AUTH_TAG_LENGTH_BYTE = 16;

Boolean WechatHandlerNotifyUrlVerify(
    ServerResponse *response,
    Stringex certificates)
{
    Boolean err = Boolean_false;
    zlog_category_t *log = response->log;
    if (!response)
    {
        zlog_warn(log, "ServerResponse *response 参数不能为空");
        return err;
    }
    if (!Stringex_IsString(certificates) && !Stringex_IsEmpty(certificates))
    {
        zlog_warn(log, "Stringex certificates 参数不能为空\r\n");
        return err;
    }
    Stringex Wechatpay_Nonce = stringex_null_command;
    Stringex Wechatpay_Signature = stringex_null_command;
    Stringex Wechatpay_Timestamp = stringex_null_command;
    Stringex plainText = stringex_null_command;
    ServerResponseHeadersValue(&Wechatpay_Nonce, response, "Wechatpay-Nonce");
    ServerResponseHeadersValue(&Wechatpay_Signature, response, "Wechatpay-Signature");
    ServerResponseHeadersValue(&Wechatpay_Timestamp, response, "Wechatpay-Timestamp");
    if (!Stringex_IsString(Wechatpay_Nonce) && !Stringex_IsEmpty(Wechatpay_Nonce))
    {
        zlog_error(log, "Wechatpay-Nonce 获取失败");
        return err;
    }
    if (!Stringex_IsString(Wechatpay_Signature) && !Stringex_IsEmpty(Wechatpay_Signature))
    {
        zlog_error(log, "Wechatpay-Signature 获取失败");
        return err;
    }
    if (!Stringex_IsString(Wechatpay_Timestamp) && !Stringex_IsEmpty(Wechatpay_Timestamp))
    {
        zlog_error(log, "Wechatpay-Timestamp 获取失败");
        return err;
    }
    size_t plainbyteslength = 18;
    if (Wechatpay_Nonce.valuestring && Wechatpay_Signature.valuestring && Wechatpay_Timestamp.valuestring)
    {
        plainbyteslength += Wechatpay_Nonce.valuelength;
        plainbyteslength += Wechatpay_Signature.valuelength;
        plainbyteslength += Wechatpay_Timestamp.valuelength;
        plainbyteslength += response->body.valuelength;
        char plainbytes[plainbyteslength];
        memset(plainbytes, 0x00, plainbyteslength);
        strcat(plainbytes, Wechatpay_Timestamp.valuestring);
        strcat(plainbytes, "\n");
        strcat(plainbytes, Wechatpay_Nonce.valuestring);
        strcat(plainbytes, "\n");
        strcat(plainbytes, response->body.valuestring);
        strcat(plainbytes, "\n");

        StringexCreate(&plainText, plainbytes, strlen(plainbytes));
        if (Stringex_Sha256WithRSA_Certificates_Verify(certificates, plainText, Wechatpay_Signature))
        {
            err = Boolean_true;
        }
        StringexDelete(plainText);
        StringexDelete(Wechatpay_Nonce);
        StringexDelete(Wechatpay_Signature);
        StringexDelete(Wechatpay_Timestamp);
    }

    return err;
}

void WechatHandlerNotifyUrl(
    struct evhttp_request *request,
    void *arg)
{
    Boolean err = Boolean_false;

    Stringex apiv3_key = ((HTTPDServer *)arg)->wConfig->apiv3_key;
    Stringex apiclient_key = ((HTTPDServer *)arg)->wConfig->apiclient_key;
    Stringex apiclient_cert = ((HTTPDServer *)arg)->wConfig->apiclient_cert;
    Stringex certificates = ((HTTPDServer *)arg)->configCertificates.certificates;
    zlog_category_t *log = ((HTTPDServer *)arg)->log;
    ServerResponse *response;
    ServerResponseCreate(&response, log, request, arg);

    if (WechatHandlerNotifyUrlVerify(response, certificates))
    {
        cJSON *postdata = cJSON_Parse(response->body.valuestring);
        if (postdata)
        {
            cJSON *resource = cJSON_GetObjectItem(postdata, "resource");
            if (resource)
            {
                cJSON *associated_data = cJSON_GetObjectItem(resource, "associated_data");
                cJSON *nonceStr = cJSON_GetObjectItem(resource, "nonce");
                cJSON *ciphertext = cJSON_GetObjectItem(resource, "ciphertext");
                Stringex cipherdata_bytes;
                Stringex auth_tag_bytes;
                Stringex ciphertext_bytes;

                unsigned char plaintext[1024] = {0};
                if (associated_data && nonceStr && ciphertext)
                {
                    StringexBase64Decrypt(&ciphertext_bytes, ciphertext->valuestring, strlen(ciphertext->valuestring), false);
                    if (ciphertext_bytes.valuestring)
                    {

                        if (ciphertext_bytes.valuelength > AUTH_TAG_LENGTH_BYTE)
                        {

                            size_t flag = ciphertext_bytes.valuelength - AUTH_TAG_LENGTH_BYTE;
                            StringexSlice(&cipherdata_bytes, ciphertext_bytes, 0, flag);
                            StringexSlice(&auth_tag_bytes, ciphertext_bytes, flag, ciphertext_bytes.valuelength);

                            int ret = aes_256_gcm_decrypt(cipherdata_bytes.valuestring, cipherdata_bytes.valuelength,
                                                          associated_data->valuestring, strlen(associated_data->valuestring), auth_tag_bytes.valuestring,
                                                          apiv3_key.valuestring, nonceStr->valuestring, strlen(nonceStr->valuestring), plaintext);
                            if (ret != -1)
                            {
                                err = Boolean_true;
                            }
                            else
                            {
                                zlog_error(response->log, "密文解密失败");
                            }

                            StringexDelete(cipherdata_bytes);
                            StringexDelete(auth_tag_bytes);
                            StringexDelete(ciphertext_bytes);
                        }
                    }
                    cJSON_free(associated_data);
                    cJSON_free(nonceStr);
                    cJSON_free(ciphertext);
                }
                cJSON_free(resource);
            }
             cJSON_free(postdata);
        }
    }

    cJSON *root = cJSON_CreateObject();
    if (root)
    {
        if (err == Boolean_true)
        {
            cJSON_AddStringToObject(root, "code", "SUCCESS");
            cJSON_AddStringToObject(root, "message", "成功");
            char *datastring = cJSON_PrintUnformatted(root);
            ServerResponseStringComplete(response, 200, datastring);
            StringDelete(datastring);
        }
        else
        {
            cJSON_AddStringToObject(root, "code", "FAIL");
            cJSON_AddStringToObject(root, "message", "失败");
            char *datastring = cJSON_PrintUnformatted(root);
            ServerResponseStringComplete(response, 400, datastring);
            StringDelete(datastring);
        }
        cJSON_free(root);
    }
    ServerResponseDelete(response);
   
}
