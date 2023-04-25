#if !defined(INCLUDE_WECHATCONFIGCERTIFICATES_H)
#define INCLUDE_WECHATCONFIGCERTIFICATES_H
#include <string.h>
#include <event.h>
// for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <unistd.h>
#include <cJSON.h>

#include <Stringex.h>
#include <aes_256_gcm.h>
#include <Stringex.h>
#include <StringexBase64.h>

typedef struct WechatConfigCertificates_t
{
    Stringex effective_time;
    Stringex expire_time;
    Stringex certificates;
    Stringex serial_no;
} WechatConfigCertificates;

Boolean WechatConfigCertificatesCreate(WechatConfigCertificates *configCertificates, Stringex mchid, Stringex serial_no, Stringex apiclient_key, Stringex apiv3_key);
void WechatConfigCertificatesDelete(WechatConfigCertificates configCertificates);
#endif