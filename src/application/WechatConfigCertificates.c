#include <WechatConfigCertificates.h>
#include <WechatHttps.h>
#include <Stringex.h>
#include <HTTPSUtils.h>
#include <StringexSha256WithRsa.h>
#include <WechatConfigAuthorization.h>
Boolean HTTPSResponseV3Certificates(
    WechatConfigCertificates *configCertificates,
    Stringex apiv3_key,
    char *monitor);

Boolean WechatConfigCertificatesCreate(
    WechatConfigCertificates *configCertificates,
    Stringex mchid,
    Stringex serial_no,
    Stringex apiclient_key,
    Stringex apiv3_key)
{
    Boolean err = Boolean_false;
    if (!configCertificates)
        return err;
    if (!Stringex_IsString(apiclient_key) && !Stringex_IsEmpty(apiclient_key))
        return err;
    if (!Stringex_IsString(mchid) && !Stringex_IsEmpty(mchid))
        return err;
    if (!Stringex_IsString(serial_no) && !Stringex_IsEmpty(serial_no))
        return err;
    if (!Stringex_IsString(apiv3_key) && !Stringex_IsEmpty(apiv3_key))
        return err;
    HTTPSUtilsCallback callback;
    Stringex auth;
    if (!WechatConfigAuthorizationBuild(&auth, "GET", "/v3/certificates", apiclient_key, mchid, serial_no, Stringex_null))
    {
        fprintf(stderr,"获取平台证书的Authorization构建错误");
        return err;
    }

    HTTPSUtilsRequestGet("https://api.mch.weixin.qq.com/v3/certificates", auth, &callback);
    err = HTTPSResponseV3Certificates(configCertificates, apiv3_key, callback.memory);
    StringexDelete(auth);
    StringDelete(callback.memory);
    return err;
}

Boolean HTTPSResponseV3Certificates(
    WechatConfigCertificates *configCertificates,
    Stringex apiv3_key,
    char *monitor)
{
    Boolean err = Boolean_false;
    if (!configCertificates)
        return err;
    if (!Stringex_IsString(apiv3_key) && !Stringex_IsEmpty(apiv3_key))
        return err;
    if (!monitor)
        return err;

    const cJSON *resolution = NULL;
    const cJSON *resolutions = NULL;

    cJSON *monitor_json = cJSON_Parse(monitor);
    if (monitor_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        err = Boolean_false;
        goto end;
    }

    resolutions = cJSON_GetObjectItemCaseSensitive(monitor_json, "data");
    cJSON_ArrayForEach(resolution, resolutions)
    {
        cJSON *serial_no = cJSON_GetObjectItemCaseSensitive(resolution, "serial_no");
        if (cJSON_IsString(serial_no) && (serial_no->valuestring != NULL))
        {
          //  printf("Checking monitor \"serial_no=>%s\"\n", serial_no->valuestring);
            StringexCreate(&configCertificates->serial_no, serial_no->valuestring, strlen(serial_no->valuestring));
        }
        cJSON *effective_time = cJSON_GetObjectItemCaseSensitive(resolution, "effective_time");
        if (cJSON_IsString(effective_time) && (effective_time->valuestring != NULL))
        {
           // printf("Checking monitor \"effective_time=>%s\"\n", effective_time->valuestring);
            StringexCreate(&configCertificates->effective_time, effective_time->valuestring, strlen(effective_time->valuestring));
        }
        cJSON *expire_time = cJSON_GetObjectItemCaseSensitive(resolution, "expire_time");
        if (cJSON_IsString(expire_time) && (expire_time->valuestring != NULL))
        {
           // printf("Checking monitor \"expire_time=>%s\"\n", expire_time->valuestring);
            StringexCreate(&configCertificates->expire_time, expire_time->valuestring, strlen(expire_time->valuestring));
        }

        cJSON *encrypt_certificate = cJSON_GetObjectItemCaseSensitive(resolution, "encrypt_certificate");
        if (encrypt_certificate)
        {
            cJSON *algorithm = cJSON_GetObjectItemCaseSensitive(encrypt_certificate, "algorithm");
            cJSON *associated_data = cJSON_GetObjectItemCaseSensitive(encrypt_certificate, "associated_data");
            cJSON *ciphertext = cJSON_GetObjectItemCaseSensitive(encrypt_certificate, "ciphertext");
            cJSON *nonceStr = cJSON_GetObjectItemCaseSensitive(encrypt_certificate, "nonce");
            if (cJSON_IsString(algorithm) && (algorithm->valuestring != NULL) &&
                cJSON_IsString(associated_data) && (associated_data->valuestring != NULL) &&
                cJSON_IsString(ciphertext) && (ciphertext->valuestring != NULL) &&
                cJSON_IsString(nonceStr) && (nonceStr->valuestring != NULL))
            {
                Stringex ciphertext_bytes = stringex_null_command;
                StringexBase64Decrypt(&ciphertext_bytes, ciphertext->valuestring, strlen(ciphertext->valuestring), false);
               // hex_print(ciphertext_bytes.valuestring, ciphertext_bytes.valuelength);
                if (ciphertext_bytes.valuestring)
                {
                    Stringex auth_tag_bytes = stringex_null_command;
                    Stringex cipherdata_bytes = stringex_null_command;
                    char plaintext[2048] = {0};
                    size_t flag = ciphertext_bytes.valuelength - 16;
                    StringexSlice(&cipherdata_bytes, ciphertext_bytes, 0, flag);
                    StringexSlice(&auth_tag_bytes, ciphertext_bytes, flag, ciphertext_bytes.valuelength);
                    int ret = aes_256_gcm_decrypt(cipherdata_bytes.valuestring, cipherdata_bytes.valuelength,
                                                  associated_data->valuestring, strlen(associated_data->valuestring), auth_tag_bytes.valuestring,
                                                  apiv3_key.valuestring, nonceStr->valuestring, strlen(nonceStr->valuestring), plaintext);
                    if (ret != -1)
                    {
                      //  printf("\t%s\r\n", plaintext);
                        if (StringexCreate(&configCertificates->certificates, plaintext, ret))
                        {
                            err = Boolean_true;
                        }
                        else
                        {
                            err = Boolean_false;
                        }
                    }
                    else
                    {
                       // printf("\t%s\r\n", "解密失败");
                        err = Boolean_false;
                    }
                    StringexDelete(ciphertext_bytes);
                    StringexDelete(auth_tag_bytes);
                    StringexDelete(cipherdata_bytes);
                }
            }
        }
    }

    goto end;

end:
    cJSON_Delete(monitor_json);
    return err;
}

void WechatConfigCertificatesDelete(WechatConfigCertificates configCertificates)
{
    if (configCertificates.serial_no.valuestring)
    {
        StringexDelete(configCertificates.serial_no);
    }
    if (configCertificates.certificates.valuestring)
    {
        StringexDelete(configCertificates.certificates);
    }
    if (configCertificates.effective_time.valuestring)
    {
        StringexDelete(configCertificates.effective_time);
    }
    if (configCertificates.expire_time.valuestring)
    {
        StringexDelete(configCertificates.expire_time);
    }
}