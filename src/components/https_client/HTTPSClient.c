#include <HTTPSClient.h>


static int ignore_cert = 0;
static int ipv6 = 0;
static int ipv4 = 0;
static void http_request_done(struct evhttp_request *request, void *ctx)
{
	// char buffer[256];
	int nread;
	HTTPSClient_t *HTTPSClient = (HTTPSClient_t *)ctx;
	memset(&HTTPSClient->HTTPSClient_response, 0x00, sizeof(HTTPSClient->HTTPSClient_response));
	if (!request || !evhttp_request_get_response_code(request))
	{
		/* If req is NULL, it means an error occurred, but
		 * sadly we are mostly left guessing what the error
		 * might have been.  We'll do our best... */
		// struct bufferevent *bev = (struct bufferevent *)ctx;
		unsigned long oslerr;
		int printed_err = 0;
		int errcode = EVUTIL_SOCKET_ERROR();
		fprintf(stderr, "some request failed - no idea which one though!\n");
		/* Print out the OpenSSL error queue that libevent
		 * squirreled away for us, if any. */

		while ((oslerr = bufferevent_get_openssl_error(HTTPSClient->bev)))
		{
			ERR_error_string_n(oslerr, HTTPSClient->HTTPSClient_response.body_buffer, sizeof(HTTPSClient->HTTPSClient_response.body_buffer));

			fprintf(stderr, "%s\n", HTTPSClient->HTTPSClient_response.body_buffer);
			printed_err = 1;
		}
		/* If the OpenSSL error queue was empty, maybe it was a
		 * socket error; let's try printing that. */
		if (!printed_err)
			fprintf(stderr, "socket error = %s (%d)\n",
					evutil_socket_error_to_string(errcode),
					errcode);
		return;
	}
	HTTPSClient->HTTPSClient_response.code = evhttp_request_get_response_code(request);
	HTTPSClient->HTTPSClient_response.code_line = strdup(evhttp_request_get_response_code_line(request));
	// fprintf(stderr, "Response line: %d %s\n",
	// 		evhttp_request_get_response_code(req),
	// 		evhttp_request_get_response_code_line(req));

	// while ((nread = evbuffer_remove(evhttp_request_get_input_buffer(req),HTTPSClient_response->buffer, sizeof(HTTPSClient_response->buffer))) > 0)
	// {
	// 	/* These are just arbitrary chunks of 256 bytes.
	// 	 * They are not lines, so we can't treat them as such. */
	// 	 fwrite(HTTPSClient_response->buffer, nread, 1, stdout);
	// }
	evbuffer_remove(evhttp_request_get_input_buffer(request), HTTPSClient->HTTPSClient_response.body_buffer, sizeof(HTTPSClient->HTTPSClient_response.body_buffer));
	// HTTPSClient_response->body_buffer = evhttp_request_get_input_buffer(req);
}

/* See http://archives.seul.org/libevent/users/Jan-2013/msg00039.html */
static int cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg)
{
	HTTPSClient_t *HTTPSClient = (HTTPSClient_t *)arg;
	// char cert_str[256];
	// const char *host = (const char *)arg;
	const char *res_str = "X509_verify_cert failed";
	// HostnameValidationResult res = Error;
	HTTPSClient->HTTPSClient_response.result = Error;
	int ok_so_far = 0;
	X509 *server_cert = NULL;
	if (ignore_cert)
	{
		return 1;
	}
	ok_so_far = X509_verify_cert(x509_ctx);
	server_cert = X509_STORE_CTX_get_current_cert(x509_ctx);
	if (ok_so_far)
	{
		HTTPSClient->HTTPSClient_response.result = validate_hostname(HTTPSClient->host, server_cert);

		switch (HTTPSClient->HTTPSClient_response.result)
		{
		case MatchFound:
			res_str = "MatchFound";
			break;
		case MatchNotFound:
			res_str = "MatchNotFound";
			break;
		case NoSANPresent:
			res_str = "NoSANPresent";
			break;
		case MalformedCertificate:
			res_str = "MalformedCertificate";
			break;
		case Error:
			res_str = "Error";
			break;
		default:
			res_str = "WTF!";
			break;
		}
	}

	X509_NAME_oneline(X509_get_subject_name(server_cert),
					  HTTPSClient->certificate, sizeof(HTTPSClient->certificate));

	if (HTTPSClient->HTTPSClient_response.result == MatchFound)
	{
		printf("https server '%s' has this certificate, "
			   "which looks good to me:\n%s\n",
			   HTTPSClient->host, HTTPSClient->certificate);
		return 1;
	}
	else
	{
		printf("Got '%s' for hostname '%s' and certificate:\n%s\n",
			   res_str, HTTPSClient->host, HTTPSClient->certificate);
		return 0;
	}
}

ok_t HTTPSClient_post(HTTPSClient_t *HTTPSClient, const char *url, char *authorization, char *data, size_t data_size)
{
	if (!HTTPSClient && !url)
	{
		return ArgumentException;
	}

	char *crt = NULL;
	if (!HTTPSClient->SystemConfig)
	{
		return NullPointerException;
	}
	SystemConfig_t *SystemConfig = HTTPSClient->SystemConfig;
	HTTPSClient->url = strdup(url);

	int r;
	const char *data_file = NULL;
	int retries = 0;
	int i;
	int ret = 0;
	enum
	{
		HTTP,
		HTTPS
	} type = HTTP;

	HTTPSClient->http_uri = evhttp_uri_parse(HTTPSClient->url);
	if (HTTPSClient->http_uri == NULL)
	{
		SystemLog_error(SystemConfig->SystemLog, "malformed url %s", HTTPSClient->url);
		return ErrorException;
	}

	HTTPSClient->scheme = (char *)evhttp_uri_get_scheme(HTTPSClient->http_uri);
	if (HTTPSClient->scheme == NULL || (strcasecmp(HTTPSClient->scheme, "https") != 0 &&
										strcasecmp(HTTPSClient->scheme, "http") != 0))
	{
		SystemLog_error(SystemConfig->SystemLog, "url must be http or https url %s", HTTPSClient->url);
		return ErrorException;
	}

	HTTPSClient->host = (char *)evhttp_uri_get_host(HTTPSClient->http_uri);
	if (HTTPSClient->host == NULL)
	{
		SystemLog_error(SystemConfig->SystemLog, "url must have a host url %s", url);
		return ErrorException;
	}

	HTTPSClient->port = evhttp_uri_get_port(HTTPSClient->http_uri);
	if (HTTPSClient->port == -1)
	{
		HTTPSClient->port = (strcasecmp(HTTPSClient->scheme, "http") == 0) ? 80 : 443;
	}

	HTTPSClient->path = (char *)evhttp_uri_get_path(HTTPSClient->http_uri);
	if (strlen(HTTPSClient->path) == 0)
	{
		HTTPSClient->path = "/";
	}

	HTTPSClient->query = (char *)evhttp_uri_get_query(HTTPSClient->http_uri);
	if (HTTPSClient->query == NULL)
	{
		snprintf(HTTPSClient->uri, sizeof(HTTPSClient->uri) - 1, "%s", HTTPSClient->path);
	}
	else
	{
		snprintf(HTTPSClient->uri, sizeof(HTTPSClient->uri) - 1, "%s?%s", HTTPSClient->path, HTTPSClient->query);
	}
	HTTPSClient->uri[sizeof(HTTPSClient->uri) - 1] = '\0';

#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
	// Initialize OpenSSL
	SSL_library_init();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
#endif

	/* This isn't strictly necessary... OpenSSL performs RAND_poll
	 * automatically on first use of random number generator. */
	r = RAND_poll();
	if (r == 0)
	{
		SystemLog_error(SystemConfig->SystemLog, "RAND_poll url %s", HTTPSClient->url);
		return ErrorException;
	}

	/* Create a new OpenSSL context */
	HTTPSClient->ssl_ctx = SSL_CTX_new(SSLv23_method());
	if (!HTTPSClient->ssl_ctx)
	{

		SystemLog_error(SystemConfig->SystemLog, "SSL_CTX_new url %s", HTTPSClient->url);
		return ErrorException;
	}

	if (crt == NULL)
	{
		X509_STORE *store;
		/* Attempt to use the system's trusted root certificates. */
		store = SSL_CTX_get_cert_store(HTTPSClient->ssl_ctx);

		if (X509_STORE_set_default_paths(store) != 1)
		{

			SystemLog_error(SystemConfig->SystemLog, "X509_STORE_set_default_paths url %s", HTTPSClient->url);
			return ErrorException;
		}
	}
	else
	{
		if (SSL_CTX_load_verify_locations(HTTPSClient->ssl_ctx, crt, NULL) != 1)
		{

			SystemLog_error(SystemConfig->SystemLog, "SSL_CTX_load_verify_locations url %s", HTTPSClient->url);
			return ErrorException;
		}
	}
	SSL_CTX_set_verify(HTTPSClient->ssl_ctx, SSL_VERIFY_PEER, NULL);
	SSL_CTX_set_cert_verify_callback(HTTPSClient->ssl_ctx, cert_verify_callback, HTTPSClient);

	// Create event base
	HTTPSClient->base = event_base_new();
	if (!HTTPSClient->base)
	{

		SystemLog_error(SystemConfig->SystemLog, "event_base_new() url %s", HTTPSClient->url);
		return ErrorException;
	}

	// Create OpenSSL bufferevent and stack evhttp on top of it
	HTTPSClient->ssl = SSL_new(HTTPSClient->ssl_ctx);
	if (HTTPSClient->ssl == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "SSL_new() url %s", HTTPSClient->url);
		return ErrorException;
	}

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
	// Set hostname for SNI extension
	SSL_set_tlsext_host_name(HTTPSClient->ssl, HTTPSClient->host);
#endif

	if (strcasecmp(HTTPSClient->scheme, "http") == 0)
	{
		HTTPSClient->bev = bufferevent_socket_new(HTTPSClient->base, -1, BEV_OPT_CLOSE_ON_FREE);
	}
	else
	{

		type = HTTPS;
		HTTPSClient->bev = bufferevent_openssl_socket_new(

			HTTPSClient->base, -1, HTTPSClient->ssl,
			BUFFEREVENT_SSL_CONNECTING,
			BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
	}

	if (HTTPSClient->bev == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "bufferevent_openssl_socket_new() url %s", url);
		return ErrorException;
	}

	bufferevent_openssl_set_allow_dirty_shutdown(HTTPSClient->bev, 1);

	// For simplicity, we let DNS resolution block. Everything else should be
	// asynchronous though.
	{
		if (HTTPSClient->host[0] == '[' && strlen(HTTPSClient->host) > 2 && ipv6)
		{
			// trim '[' and ']'
			char *host_ipv6 = strndup(&HTTPSClient->host[1], strlen(&HTTPSClient->host[1]) - 1);
			HTTPSClient->connection = evhttp_connection_base_bufferevent_new(HTTPSClient->base, NULL, HTTPSClient->bev,
																			 host_ipv6, HTTPSClient->port);
			free(host_ipv6);
		}
		else
		{
			HTTPSClient->connection = evhttp_connection_base_bufferevent_new(HTTPSClient->base, NULL, HTTPSClient->bev,
																			 HTTPSClient->host, HTTPSClient->port);
		}
	}
	if (HTTPSClient->connection == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "evhttp_connection_base_bufferevent_new() failed");
		return ErrorException;
	}

	if (ipv4)
	{
		evhttp_connection_set_family(HTTPSClient->connection, AF_INET);
	}
	if (ipv6)
	{
		evhttp_connection_set_family(HTTPSClient->connection, AF_INET6);
	}

	if (retries > 0)
	{
		evhttp_connection_set_retries(HTTPSClient->connection, retries);
	}
	if (HTTPSClient->timeout >= 0)
	{
		evhttp_connection_set_timeout(HTTPSClient->connection, HTTPSClient->timeout);
	}

	// Fire off the request
	HTTPSClient->request = evhttp_request_new(http_request_done, HTTPSClient);
	if (HTTPSClient->request == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "evhttp_request_new() failed");
		return ErrorException;
	}

	HTTPSClient->output_headers = evhttp_request_get_output_headers(HTTPSClient->request);
	evhttp_add_header(HTTPSClient->output_headers, "Host", HTTPSClient->host);
	evhttp_add_header(HTTPSClient->output_headers, "Content-Type", HTTPSClient->contentType);
	evhttp_add_header(HTTPSClient->output_headers, "Accept", HTTPSClient->contentType);
	evhttp_add_header(HTTPSClient->output_headers, "User-Agent", HTTPSClient->userAgent);
	if (authorization)
	{
		evhttp_add_header(HTTPSClient->output_headers, "Authorization", authorization);
	}

	evhttp_add_header(HTTPSClient->output_headers, "Connection", "close");
	if (data && data_size > 0)
	{

		HTTPSClient->output_buffer = evhttp_request_get_output_buffer(HTTPSClient->request);
		evbuffer_add(HTTPSClient->output_buffer, data, data_size);
	}
	else
	{
		data_size = 0;
	}
	evutil_snprintf(HTTPSClient->output_buffer_size, sizeof(HTTPSClient->output_buffer_size) - 1, "%lu", (unsigned long)data_size);
	evhttp_add_header(HTTPSClient->output_headers, "Content-Length", HTTPSClient->output_buffer_size);

	r = evhttp_make_request(HTTPSClient->connection, HTTPSClient->request, EVHTTP_REQ_POST, HTTPSClient->uri);
	if (r != 0)
	{

		SystemLog_error(SystemConfig->SystemLog, "evhttp_make_request() failed");
		return ErrorException;
	}

	ret = event_base_dispatch(HTTPSClient->base);

	if (HTTPSClient->connection)
		evhttp_connection_free(HTTPSClient->connection);
	if (HTTPSClient->http_uri)
		evhttp_uri_free(HTTPSClient->http_uri);
	if (HTTPSClient->base)
		event_base_free(HTTPSClient->base);

	if (HTTPSClient->ssl_ctx)
		SSL_CTX_free(HTTPSClient->ssl_ctx);
	if (type == HTTP && HTTPSClient->ssl)
		SSL_free(HTTPSClient->ssl);
#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
	EVP_cleanup();
	ERR_free_strings();

#if OPENSSL_VERSION_NUMBER < 0x10000000L
	ERR_remove_state(0);
#else
	ERR_remove_thread_state(NULL);
#endif

	CRYPTO_cleanup_all_ex_data();

	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
#endif /* (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L) */

	return ret;
}

/**GET request http*/
ok_t HTTPSClient_get(HTTPSClient_t *HTTPSClient, const char *url)
{

	if (!HTTPSClient && !url)
	{
		return ArgumentException;
	}
	char *crt = NULL;
	if (!HTTPSClient->SystemConfig)
	{
		return NullPointerException;
	}
	SystemConfig_t *SystemConfig = HTTPSClient->SystemConfig;
	HTTPSClient->url = strdup(url);

	int r;
	const char *data_file = NULL;
	int retries = 0;
	int i;
	int ret = 0;
	enum
	{
		HTTP,
		HTTPS
	} type = HTTP;

	HTTPSClient->http_uri = evhttp_uri_parse(HTTPSClient->url);
	if (HTTPSClient->http_uri == NULL)
	{
		SystemLog_error(SystemConfig->SystemLog, "malformed url %s", HTTPSClient->url);
		return ErrorException;
	}

	HTTPSClient->scheme = (char *)evhttp_uri_get_scheme(HTTPSClient->http_uri);
	if (HTTPSClient->scheme == NULL || (strcasecmp(HTTPSClient->scheme, "https") != 0 &&
										strcasecmp(HTTPSClient->scheme, "http") != 0))
	{
		SystemLog_error(SystemConfig->SystemLog, "url must be http or https url %s", HTTPSClient->url);
		return ErrorException;
	}

	HTTPSClient->host = (char *)evhttp_uri_get_host(HTTPSClient->http_uri);
	if (HTTPSClient->host == NULL)
	{
		SystemLog_error(SystemConfig->SystemLog, "url must have a host url %s", url);
		return ErrorException;
	}

	HTTPSClient->port = evhttp_uri_get_port(HTTPSClient->http_uri);
	if (HTTPSClient->port == -1)
	{
		HTTPSClient->port = (strcasecmp(HTTPSClient->scheme, "http") == 0) ? 80 : 443;
	}

	HTTPSClient->path = (char *)evhttp_uri_get_path(HTTPSClient->http_uri);
	if (strlen(HTTPSClient->path) == 0)
	{
		HTTPSClient->path = "/";
	}

	HTTPSClient->query = (char *)evhttp_uri_get_query(HTTPSClient->http_uri);
	if (HTTPSClient->query == NULL)
	{
		snprintf(HTTPSClient->uri, sizeof(HTTPSClient->uri) - 1, "%s", HTTPSClient->path);
	}
	else
	{
		snprintf(HTTPSClient->uri, sizeof(HTTPSClient->uri) - 1, "%s?%s", HTTPSClient->path, HTTPSClient->query);
	}
	HTTPSClient->uri[sizeof(HTTPSClient->uri) - 1] = '\0';

#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
	// Initialize OpenSSL
	SSL_library_init();
	ERR_load_crypto_strings();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();
#endif

	/* This isn't strictly necessary... OpenSSL performs RAND_poll
	 * automatically on first use of random number generator. */
	r = RAND_poll();
	if (r == 0)
	{
		SystemLog_error(SystemConfig->SystemLog, "RAND_poll url %s", HTTPSClient->url);
		return ErrorException;
	}

	/* Create a new OpenSSL context */
	HTTPSClient->ssl_ctx = SSL_CTX_new(SSLv23_method());
	if (!HTTPSClient->ssl_ctx)
	{

		SystemLog_error(SystemConfig->SystemLog, "SSL_CTX_new url %s", HTTPSClient->url);
		return ErrorException;
	}

	if (crt == NULL)
	{
		X509_STORE *store;
		/* Attempt to use the system's trusted root certificates. */
		store = SSL_CTX_get_cert_store(HTTPSClient->ssl_ctx);

		if (X509_STORE_set_default_paths(store) != 1)
		{

			SystemLog_error(SystemConfig->SystemLog, "X509_STORE_set_default_paths url %s", HTTPSClient->url);
			return ErrorException;
		}
	}
	else
	{
		if (SSL_CTX_load_verify_locations(HTTPSClient->ssl_ctx, crt, NULL) != 1)
		{

			SystemLog_error(SystemConfig->SystemLog, "SSL_CTX_load_verify_locations url %s", HTTPSClient->url);
			return ErrorException;
		}
	}
	SSL_CTX_set_verify(HTTPSClient->ssl_ctx, SSL_VERIFY_PEER, NULL);
	SSL_CTX_set_cert_verify_callback(HTTPSClient->ssl_ctx, cert_verify_callback, HTTPSClient);

	// Create event base
	HTTPSClient->base = event_base_new();
	if (!HTTPSClient->base)
	{

		SystemLog_error(SystemConfig->SystemLog, "event_base_new() url %s", HTTPSClient->url);
		return ErrorException;
	}

	// Create OpenSSL bufferevent and stack evhttp on top of it
	HTTPSClient->ssl = SSL_new(HTTPSClient->ssl_ctx);
	if (HTTPSClient->ssl == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "SSL_new() url %s", HTTPSClient->url);
		return ErrorException;
	}

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
	// Set hostname for SNI extension
	SSL_set_tlsext_host_name(HTTPSClient->ssl, HTTPSClient->host);
#endif

	if (strcasecmp(HTTPSClient->scheme, "http") == 0)
	{
		HTTPSClient->bev = bufferevent_socket_new(HTTPSClient->base, -1, BEV_OPT_CLOSE_ON_FREE);
	}
	else
	{

		type = HTTPS;
		HTTPSClient->bev = bufferevent_openssl_socket_new(

			HTTPSClient->base, -1, HTTPSClient->ssl,
			BUFFEREVENT_SSL_CONNECTING,
			BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
	}

	if (HTTPSClient->bev == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "bufferevent_openssl_socket_new() url %s", url);
		return ErrorException;
	}

	bufferevent_openssl_set_allow_dirty_shutdown(HTTPSClient->bev, 1);

	// For simplicity, we let DNS resolution block. Everything else should be
	// asynchronous though.
	{
		if (HTTPSClient->host[0] == '[' && strlen(HTTPSClient->host) > 2 && ipv6)
		{
			// trim '[' and ']'
			char *host_ipv6 = strndup(&HTTPSClient->host[1], strlen(&HTTPSClient->host[1]) - 1);
			HTTPSClient->connection = evhttp_connection_base_bufferevent_new(HTTPSClient->base, NULL, HTTPSClient->bev,
																			 host_ipv6, HTTPSClient->port);
			free(host_ipv6);
		}
		else
		{
			HTTPSClient->connection = evhttp_connection_base_bufferevent_new(HTTPSClient->base, NULL, HTTPSClient->bev,
																			 HTTPSClient->host, HTTPSClient->port);
		}
	}
	if (HTTPSClient->connection == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "evhttp_connection_base_bufferevent_new() failed");
		return ErrorException;
	}

	if (ipv4)
	{
		evhttp_connection_set_family(HTTPSClient->connection, AF_INET);
	}
	if (ipv6)
	{
		evhttp_connection_set_family(HTTPSClient->connection, AF_INET6);
	}

	if (retries > 0)
	{
		evhttp_connection_set_retries(HTTPSClient->connection, retries);
	}
	if (HTTPSClient->timeout >= 0)
	{
		evhttp_connection_set_timeout(HTTPSClient->connection, HTTPSClient->timeout);
	}

	// Fire off the request
	HTTPSClient->request = evhttp_request_new(http_request_done, HTTPSClient);
	if (HTTPSClient->request == NULL)
	{

		SystemLog_error(SystemConfig->SystemLog, "evhttp_request_new() failed");
		return ErrorException;
	}
	struct evkeyvalq *output_headers;
	struct evbuffer *output_buffer;
	output_headers = evhttp_request_get_output_headers(HTTPSClient->request);

	evhttp_add_header(output_headers, "Host", HTTPSClient->host);
	evhttp_add_header(output_headers, "Connection", "close");

	if (data_file)
	{
		/* NOTE: In production code, you'd probably want to use
		 * evbuffer_add_file() or evbuffer_add_file_segment(), to
		 * avoid needless copying. */
		FILE *f = fopen(data_file, "rb");
		char buf[1024];
		size_t s;
		size_t bytes = 0;

		output_buffer = evhttp_request_get_output_buffer(HTTPSClient->request);
		while ((s = fread(buf, 1, sizeof(buf), f)) > 0)
		{
			evbuffer_add(output_buffer, buf, s);
			bytes += s;
		}
		evutil_snprintf(buf, sizeof(buf) - 1, "%lu", (unsigned long)bytes);
		evhttp_add_header(output_headers, "Content-Length", buf);
		fclose(f);
	}
	printf("\turi[%s]\r\n", HTTPSClient->uri);
	r = evhttp_make_request(HTTPSClient->connection, HTTPSClient->request, EVHTTP_REQ_GET, HTTPSClient->uri);
	if (r != 0)
	{

		SystemLog_error(SystemConfig->SystemLog, "evhttp_make_request() failed");
		return ErrorException;
	}

	ret = event_base_dispatch(HTTPSClient->base);

	if (HTTPSClient->connection)
		evhttp_connection_free(HTTPSClient->connection);
	if (HTTPSClient->http_uri)
		evhttp_uri_free(HTTPSClient->http_uri);
	if (HTTPSClient->base)
		event_base_free(HTTPSClient->base);

	if (HTTPSClient->ssl_ctx)
		SSL_CTX_free(HTTPSClient->ssl_ctx);
	if (type == HTTP && HTTPSClient->ssl)
		SSL_free(HTTPSClient->ssl);
#if (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L)
	EVP_cleanup();
	ERR_free_strings();

#if OPENSSL_VERSION_NUMBER < 0x10000000L
	ERR_remove_state(0);
#else
	ERR_remove_thread_state(NULL);
#endif

	CRYPTO_cleanup_all_ex_data();

	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
#endif /* (OPENSSL_VERSION_NUMBER < 0x10100000L) || \
	(defined(LIBRESSL_VERSION_NUMBER) && LIBRESSL_VERSION_NUMBER < 0x20700000L) */

	return ret;
}

ok_t https_client_initializing(https_client_t **https_client, program_t *program)
{
	if (!program && !program->allocate && !SystemConfig->SystemLog && !SystemConfig->ConfUtils)
	{
		return ArgumentException;
	}
	if (!((*HTTPSClient) = AllocateUtils_pool(SystemConfig->AllocateUtils, sizeof(HTTPSClient_t))))
	{
		return NullPointerException;
	}
	memset((*HTTPSClient), 0x00, sizeof(HTTPSClient_t));
	int HTTPSClient_commands_size = sizeof(HTTPSClient_commands) / sizeof(HTTPSClient_commands[0]);
	ConfUtils_mapping(SystemConfig->ConfUtils, (void **)HTTPSClient, sizeof(HTTPSClient_t), "HTTPSCLIENT", HTTPSClient_commands, HTTPSClient_commands_size);
	(*HTTPSClient)->SystemConfig = SystemConfig;
	return OK;
}
ok_t HTTPSClient_cleanup(HTTPSClient_t *HTTPSClient, SystemConfig_t *SystemConfig)
{
	if (!HTTPSClient && !SystemConfig->AllocateUtils)
	{
		return ArgumentException;
	}
	AllocateUtils_free(SystemConfig->AllocateUtils, HTTPSClient);
	return OK;
}

ok_t HTTPSClient_authorization(HTTPSClient_t *HTTPSClient, char *authorization)
{
	if (!HTTPSClient && !authorization)
	{
		return ArgumentException;
	}
	HTTPSClient->output_headers = evhttp_request_get_output_headers(HTTPSClient->request);
	evhttp_add_header(HTTPSClient->output_headers, "Authorization", authorization);
	return OK;
}