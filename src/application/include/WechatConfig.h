#if !defined(INCLUDE_WECHATPAYMENT_H)
#define INCLUDE_WECHATPAYMENT_H
#include <wangyonglin/wangyonglin.h>
#include <string_by_this.h>
#include <cJSON.h>
#include <Stringex.h>

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

WechatConfig *WechatConfigCreate(Config_t *config, WechatConfig **wcConfig, const char *section);
void WechatConfigDelete(WechatConfig *wcConfig);

cJSON *WechatShippingJSON(cJSON **root, WechatConfig *weConfig, char *openidlString, int totalInteger, char *descriptionString);
void WechatPOSTAuth(string_by_t *authString, WechatConfig *wcConfig, cJSON *bodyJSON, string_by_t url);
#endif