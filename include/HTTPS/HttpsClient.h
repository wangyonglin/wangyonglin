#ifndef __HTTPSCLIENT__H__
#define __HTTPSCLIENT__H__
#include <wangyonglin/config.h>
#include <wangyonglin/core.h>
typedef struct
{
	char body[256];
	int lenght;
	int error_code;
	char reason[256];
} https_response_t;
typedef struct
{
	struct event_base *base;
	struct evhttp_uri *http_uri;
	struct evhttp_connection *evcon;
	struct evhttp_request *req;
	struct evkeyvalq *output_headers;
	struct evbuffer *output_buffer;
	struct bufferevent *bev;
	https_response_t reps;
} https_client_t;

int https_client(https_client_t *client, const char *url, int timeout);
#endif  //!__HTTPSCLIENT__H__