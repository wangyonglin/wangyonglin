#if !defined(__HTTPS_SERVER_H__)
#define __HTTPS_SERVER_H__
#include <string.h>
#include <event.h>
//for http
#include <evhttp.h>
#include <event2/http.h>
#include <event2/http_struct.h>
#include <event2/http_compat.h>
#include <event2/util.h>
#include <cjson/cJSON.h>
#include <unistd.h>


typedef struct{
struct evhttp *httpd;
}https_httpd_config_t;

https_httpd_config_t *httpd_initializing(https_httpd_config_t **httpd, const char *address, uint16_t port);
void httpd_cleanup(https_httpd_config_t *httpd);
#endif