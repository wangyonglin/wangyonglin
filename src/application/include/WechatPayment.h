#if !defined(INCLUDE_WECHATPAYMENT_H)
#define INCLUDE_WECHATPAYMENT_H
#include<wangyonglin/wangyonglin.h>
#include <string_by_this.h>

typedef struct _WechatPayment_t
{
    string_by_t appid;
    string_by_t secret;
    string_by_t mchid;
    string_by_t serial_no;
    string_by_t apiclient_key;
    string_by_t body;
    string_by_t httptype;
    string_by_t notify_url;
    
} WechatPayment;

WechatPayment *WechatPaymentCreate(config_t *config, WechatPayment **payment, const char *section);
void WechatPaymentDelete(WechatPayment *payment);
//下单函数
void WechatPaymentPurchaseAuthorization(WechatPayment *payment, char *OpenId,integer_by_t total, string_by_t url, string_by_t *body, string_by_t *authorization);
#endif