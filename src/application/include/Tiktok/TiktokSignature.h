#if !defined(INCLUDE_TIKTOK_TIKTOKSIGNATURE_H)
#define INCLUDE_TIKTOK_TIKTOKSIGNATURE_H
#include <MainConfig.h>
#include <Stringex.h>

Boolean TiktokSignature(
    Stringex *tiktokSignature,
    const char *http_request_type,
    const char *http_request_url,
    Stringex tiktokPublicKey,
    Stringex tiktokSignatureNoce,
    Stringex tiktokSignatureTomstamp,
    Stringex tiktokBody);

Boolean TiktokAuthorization(
    Stringex *tiktokAuthorization,
    const char *http_request_type,
    const char *http_request_url,
    Stringex tiktokAppid,
    Stringex tiktokPublicKey,
    Stringex tiktokBody);

    Boolean TiktokSignatureVerify();
#endif