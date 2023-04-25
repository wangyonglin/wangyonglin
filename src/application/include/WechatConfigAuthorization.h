#if !defined(INCLUDE_WECHATCONFIGAUTHORIZATION_H)
#define INCLUDE_WECHATCONFIGAUTHORIZATION_H
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
#include <StringexSha256WithRsa.h>

Boolean WechatConfigAuthorizationBuild(Stringex *retvalue,
                                        const char *http_request_type,
                                        const char *http_request_url,
                                        Stringex apiclient_key,
                                        Stringex mchid,
                                        Stringex serial_no,
                                        Stringex body);
#endif