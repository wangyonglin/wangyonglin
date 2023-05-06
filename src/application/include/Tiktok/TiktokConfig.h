#if !defined(INCLUDE_TIKTOK_TIKTOKCONFIG_H)
#define INCLUDE_TIKTOK_TIKTOKCONFIG_H
#include <MainConfig.h>
#include <Stringex.h>
#include <curl/curl.h>
typedef struct _TiktokCallbackResponse
{
    CURLcode code;
    char *memory;
    size_t size;
} TiktokCallbackResponse, TiktokCallbackResponse_t;

typedef struct _TiktokConfig
{
    Stringex appid;
    Stringex secret;
    Stringex public_key;
} TiktokConfig, TiktokConfig_t, *pTiktokConfig;

TiktokConfig *TiktokConfigCreate(
    TiktokConfig **tiktokConfig,
    MainConfig *mainConfig,
    const char *section);
void TiktokConfigDelete(TiktokConfig *tiktokConfig);

Boolean TiktokPostToken(
    char *urlvalue,
    Stringex access_token,
    Stringex auth,
    Stringex body,
    TiktokCallbackResponse *tiktokCallbackResponse);

Boolean TiktokPost(
    char *urlvalue,
    Stringex auth,
    Stringex body,
    TiktokCallbackResponse *tiktokCallbackResponse);
    
#endif