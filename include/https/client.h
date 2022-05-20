#ifndef __HTTPS_CLIENT__H__
#define __HTTPS_CLIENT__H__

#include <wangyonglin/linux.h>
typedef struct
{
	struct event_base *base;
	struct evhttp_uri *http_uri;
	struct evhttp_connection *evcon;
	struct evhttp_request *req;
	struct evkeyvalq *output_headers;
	struct evbuffer *output_buffer;
	struct bufferevent *bev;
	struct evbuffer *response_body;
	char response_reason[256];
	int response_error_code;
} https_client_t;

int https_client(https_client_t **client, const char *url, int timeout);
#endif  //!__HTTPSCLIENT__H__