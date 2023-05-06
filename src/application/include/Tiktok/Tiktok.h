#if !defined(INCLUDE_TIKTOK_TIKTOK_H)
#define INCLUDE_TIKTOK_TIKTOK_H
#include <MainConfig.h>
#include <Stringex.h>

#include <Tiktok/TiktokConfig.h>

void TiktokHandlerAccessToken(
    struct evhttp_request *request,
    void *arg);

void TiktokHandlerTastStart(
    struct evhttp_request *request,
    void *arg);
#endif
