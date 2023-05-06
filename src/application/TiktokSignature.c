#include <Tiktok/TiktokSignature.h>
#include <Sha256Rsa2048.h>
#include <ServerResponse.h>
Boolean TiktokSignature(
    Stringex *tiktokSignature,
    const char *http_request_type,
    const char *http_request_url,
    Stringex tiktokPublicKey,
    Stringex tiktokSignatureNoce,
    Stringex tiktokSignatureTomstamp,
    Stringex tiktokBody)
{
    Boolean err = Boolean_false;
    if (!http_request_type)
        return err;
    if (!http_request_url)
        return err;
    if (!Stringex_IsEmpty(tiktokPublicKey) && !Stringex_IsString(tiktokPublicKey))
    {
        return err;
    }
    if (!Stringex_IsEmpty(tiktokSignatureNoce) && !Stringex_IsString(tiktokSignatureNoce))
    {
        return err;
    }
    if (!Stringex_IsEmpty(tiktokSignatureTomstamp) && !Stringex_IsString(tiktokSignatureTomstamp))
    {
        return err;
    }
    int tiktokSignmakeSize = 16;
    tiktokSignmakeSize += strlen(http_request_type);
    tiktokSignmakeSize += strlen(http_request_url);
    tiktokSignmakeSize += tiktokSignatureNoce.valuelength;
    tiktokSignmakeSize += tiktokSignatureTomstamp.valuelength;
    if (Stringex_IsEmpty(tiktokBody) && Stringex_IsString(tiktokBody))
    {
        tiktokSignmakeSize += tiktokBody.valuelength;
    }

    char tiktokSignmake[tiktokSignmakeSize];
    memset(tiktokSignmake, 0x00, tiktokSignmakeSize);

    strcat(tiktokSignmake, http_request_type);
    strcat(tiktokSignmake, "\n");
    strcat(tiktokSignmake, http_request_url);
    strcat(tiktokSignmake, "\n");
    strcat(tiktokSignmake, tiktokSignatureNoce.valuestring);
    strcat(tiktokSignmake, "\n");
    strcat(tiktokSignmake, tiktokSignatureTomstamp.valuestring);
    strcat(tiktokSignmake, "\n");
    if (Stringex_IsEmpty(tiktokBody) && Stringex_IsString(tiktokBody))
    {
        strcat(tiktokSignmake, tiktokBody.valuestring);
    }
    strcat(tiktokSignmake, "\n");

    if (Sha256Rsa2048_Signature(tiktokSignature, tiktokPublicKey, tiktokSignmake))
    {
        err = Boolean_true;
    }
    return err;
}
Boolean TiktokAuthorization(
    Stringex *tiktokAuthorization,
    const char *http_request_type,
    const char *http_request_url,
    Stringex tiktokAppid,
    Stringex tiktokPublicKey,
    Stringex tiktokBody)
{
    Boolean err = Boolean_false;
    if (!Stringex_IsEmpty(tiktokPublicKey) && !Stringex_IsString(tiktokPublicKey))
    {
        return err;
    }
    if (!Stringex_IsEmpty(tiktokAppid) && !Stringex_IsString(tiktokAppid))
    {
        return err;
    }
    if (!Stringex_IsEmpty(tiktokBody) && !Stringex_IsString(tiktokBody))
    {
        return err;
    }
    Stringex tiktokSignature = Stringex_null;
    Stringex tiktokNoce = Stringex_null;
    Stringex tiktokTomstamp = Stringex_null;
    StringexNonce(&tiktokNoce, 32);
    StringexTimestamp(&tiktokTomstamp);

    if (TiktokSignature(
            &tiktokSignature,
            http_request_type,
            http_request_url,
            tiktokPublicKey,
            tiktokNoce,
            tiktokTomstamp,
            tiktokBody))
    {
        int authcount = 86;
        authcount += tiktokNoce.valuelength;
        authcount += tiktokTomstamp.valuelength;
        authcount += tiktokSignature.valuelength;
        char tmpstring[authcount];
        memset(tmpstring, 0x00, authcount);

        int tmpsize = sprintf(
            tmpstring,
            "SHA256-RSA2048 appid=\"%s\",nonce_str=\"%s\",timestamp=\"%s\",key_version=\"1\",signature=\"%s\"",
            tiktokAppid.valuestring,
            tiktokNoce.valuestring,
            tiktokTomstamp.valuestring,
            tiktokSignature.valuestring);

        if (tmpsize > 0)
        {
            if (StringexCreate(tiktokAuthorization, tmpstring, tmpsize))
            {
                err = Boolean_true;
            }
        }
    }
    StringexDelete(tiktokNoce);
    StringexDelete(tiktokTomstamp);
    StringexDelete(tiktokSignature);
    return err;
}
Boolean TiktokSignatureVerify(ServerResponse *response, Stringex certificates)
{
    Boolean err = Boolean_false;
    if (!response)
    {
        return err;
    }
    zlog_category_t *log = response->log;
    Stringex timestampStr = Stringex_null;
    Stringex nonceStr = Stringex_null;
    Stringex SignatureStr = Stringex_null;
    Stringex plainText = Stringex_null;
    if (!ServerResponseHeadersValue(&timestampStr, response, "Byte-Timestamp"))
    {
        zlog_error(log, "HTTP Heander Byte-Timestamp fail");
        return err;
    }

    if (!ServerResponseHeadersValue(&nonceStr, response, "Byte-Nonce-Str"))
    {
        StringexDelete(timestampStr);
        zlog_error(log, "HTTP Heander Byte-Nonce-Str fail");
        return err;
    }
    if (!ServerResponseHeadersValue(&SignatureStr, response, "Byte-Signature"))
    {
        StringexDelete(timestampStr);
        StringexDelete(nonceStr);
        zlog_error(log, "HTTP Heander Byte-Nonce-Str fail");
        return err;
    }
    size_t tmpStrlen = timestampStr.valuelength + 32;
    tmpStrlen += nonceStr.valuelength;
    if (Stringex_IsEmpty(response->body))
    {
        tmpStrlen += response->body.valuelength;
    }
    char tmpStr[tmpStrlen];
    memset(tmpStr, 0x00, tmpStrlen);
    strcat(tmpStr, timestampStr.valuestring);
    strcat(tmpStr, "\n");
    strcat(tmpStr, nonceStr.valuestring);
    strcat(tmpStr, "\n");
    if (Stringex_IsEmpty(response->body))
    {
        strcat(tmpStr, response->body.valuestring);
    }
    strcat(tmpStr, "\n");
    StringexCreate(&plainText, tmpStr, strlen(tmpStr));
    if (Sha256Rsa2048_Verify(SignatureStr, plainText, certificates))
    {
        err = Boolean_true;
    }
    StringexDelete(timestampStr);
    StringexDelete(nonceStr);
    StringexDelete(SignatureStr);
    StringexDelete(plainText);
    return err;
}