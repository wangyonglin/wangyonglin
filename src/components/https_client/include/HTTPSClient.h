#if !defined(INCLUDE_HTTPSCLIENT_H)
#define INCLUDE_HTTPSCLIENT_H
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include "openssl_hostname_validation.h"
#include <developer/program.h>


typedef struct
{
	char body_buffer[256];
	int code;
	char *code_line;
	HostnameValidationResult result;
} HTTPSClient_response_t;

typedef struct
{
	char *url;
	char *host;
	int port;
	char *path;
	char *scheme;
	long timeout;
	char *query;
	char uri[256];
	char certificate[256];
	struct evhttp_request *request;
	struct evhttp_connection *connection;
	struct event_base *base;
	struct evhttp_uri *http_uri;
	struct bufferevent *bev;
	struct evkeyvalq *output_headers;
	struct evbuffer *output_buffer;
	char output_buffer_size[32];
	SSL_CTX *ssl_ctx;
	SSL *ssl;
	HTTPSClient_response_t HTTPSClient_response;
	SystemConfig_t *SystemConfig;
	enum evhttp_cmd_type kind;
	char *userAgent;
	char *contentType;
} https_client_t;

typedef ok_t (*HTTPSClient_callback_t)(HTTPSClient_t *HTTPSClient, char *data, size_t data_size);

ok_t https_client_initializing(https_client_t **https_client, program_t *program);

ok_t HTTPSClient_authorization(HTTPSClient_t *HTTPSClient, char *authorization);
ok_t HTTPSClient_cleanup(HTTPSClient_t *HTTPSClient, SystemConfig_t *SystemConfig);
ok_t HTTPSClient_post(HTTPSClient_t *HTTPSClient, const char *url,char * authorization,char *data, size_t data_size);
ok_t HTTPSClient_get(HTTPSClient_t *HTTPSClient, const char *url);
#endif