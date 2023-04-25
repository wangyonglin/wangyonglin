#include <WechatConfigAuthorization.h>

Boolean WechatConfigAuthorizationBuild(Stringex *retvalue,
                                        const char *http_request_type,
                                        const char *http_request_url,
                                        Stringex apiclient_key,
                                        Stringex mchid,
                                        Stringex serial_no,
                                        Stringex body)
{
    Boolean err = Boolean_false;
    if (!http_request_type)
        return err;
    if (!http_request_url)
        return err;
    if (!Stringex_IsString(apiclient_key) && !Stringex_IsEmpty(apiclient_key))
        return err;
    if (!Stringex_IsString(mchid) && !Stringex_IsEmpty(mchid))
        return err;
    if (!Stringex_IsString(serial_no) && !Stringex_IsEmpty(serial_no))
        return err;
    Stringex timestamp = stringex_null_command;
    Stringex nonce_str = stringex_null_command;
    Stringex signature = stringex_null_command;

    StringexNonce(&nonce_str, 32);
    StringexTimestamp(&timestamp);
    Integer signStrlength = 32 + nonce_str.valuelength + timestamp.valuelength;
    if (Stringex_IsString(body) && Stringex_IsEmpty(body))
    {
        signStrlength += body.valuelength;
    }
    char signStr[signStrlength];
    memset(signStr, 0x00, sizeof(signStr));
    strcat(signStr, http_request_type);
    strcat(signStr, "\n");
    strcat(signStr, http_request_url);
    strcat(signStr, "\n");
    strcat(signStr, timestamp.valuestring);
    strcat(signStr, "\n");
    strcat(signStr, nonce_str.valuestring);
    strcat(signStr, "\n");
    if (Stringex_IsString(body) && Stringex_IsEmpty(body))
    {
        strcat(signStr, body.valuestring);
    }
    strcat(signStr, "\n");
  //  printf("\t%s\r\n", signStr);
    Stringex_Sha256WithRSA_Signature(&signature, apiclient_key, signStr);
    size_t authcount = 128 + mchid.valuelength + nonce_str.valuelength + signature.valuelength + timestamp.valuelength + serial_no.valuelength;
    char tmpstring[authcount];
    memset(tmpstring, 0x00, authcount);
    int tmpstringsize = sprintf(tmpstring, "WECHATPAY2-SHA256-RSA2048 mchid=\"%s\",nonce_str=\"%s\",signature=\"%s\",timestamp=\"%s\",serial_no=\"%s\"",
                                mchid.valuestring, nonce_str.valuestring, signature.valuestring, timestamp.valuestring, serial_no.valuestring);
    if (tmpstringsize > 0)
    {
        if (StringexCreate(retvalue, tmpstring, tmpstringsize))
            err = Boolean_true;
    }
    StringexDelete(timestamp);
    StringexDelete(nonce_str);
    StringexDelete(signature);
    return err;
}