#if !defined(INCLUDE_WECHATPAYMENT_H)
#define INCLUDE_WECHATPAYMENT_H

#include <cJSON.h>
#include <Stringex.h>
#include <MainConfig.h>
typedef struct _WechatConfig_t
{
    // 小程序 ID
    Stringex appid;
    Stringex secret;
    // 商户的商户号
    Stringex mchid;
    Stringex serial_no;
    Stringex apiclient_key;
    Stringex apiclient_cert;
    Stringex body;
    Stringex httptype;
    Stringex notify_url;
    // API v3密钥
    Stringex apiv3_key;

} WechatConfig;

WechatConfig *WechatConfigCreate(MainConfig *mainConfig, WechatConfig **wcConfig, const char *section);
void WechatConfigDelete(WechatConfig *wcConfig);

cJSON *WechatShippingJSON(cJSON **root, WechatConfig *weConfig, char *openidlString, int totalInteger, char *descriptionString);
void WechatPOSTAuth(Stringex *authString, WechatConfig *wcConfig, cJSON *bodyJSON, Stringex url);
#endif