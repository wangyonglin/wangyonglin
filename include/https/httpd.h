#ifndef __HTTPD__H__
#define __HTTPD__H__
#include <wangyonglin/linux.h>
#define HTTPS 443
#define HTTP 80
typedef struct
{
    struct event_base *base;
    struct evhttp *httpd;
   // config_t *config;
} https_d_t;



void login_handler(struct evhttp_request *req, void *arg);
#endif //!__HTTPD__H__