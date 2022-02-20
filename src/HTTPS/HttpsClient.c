#if defined(__APPLE__) && defined(__clang__)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <HTTPS/HttpsClient.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#define snprintf _snprintf
#define strcasecmp _stricmp
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

#include <event2/bufferevent_ssl.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>

#ifdef USE_MBEDTLS
#include <mbedtls/error.h>
#include <mbedtls/ssl.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/entropy.h>
#else
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#endif

#ifdef USE_MBEDTLS
#else
#include <HTTPS/openssl_hostname_validation.h>
#endif

static int ignore_cert = 0;
static int ipv6 = 0;
static int ipv4 = 0;

static void
http_request_done(struct evhttp_request *req, void *ctx)
{
	https_client_t *client = (https_client_t *)ctx;
	https_response_t *reps = &client->reps;
	bzero(reps->reason,sizeof(reps->reason));
	if (!req || !evhttp_request_get_response_code(req))
	{
		/* If req is NULL, it means an error occurred, but
		 * sadly we are mostly left guessing what the error
		 * might have been.  We'll do our best... */
		struct bufferevent *bev = (struct bufferevent *)client->bev;
		unsigned long oslerr;
		int printed_err = 0;
		reps->error_code = EVUTIL_SOCKET_ERROR();
		fprintf(stderr, "some request failed - no idea which one though!\n");
		/* Print out the OpenSSL error queue that libevent
		 * squirreled away for us, if any. */
#ifdef USE_MBEDTLS
		while ((oslerr = bufferevent_get_mbedtls_error(bev)))
		{
			mbedtls_strerror(oslerr, buffer, sizeof(buffer));
#else
		while ((oslerr = bufferevent_get_openssl_error(bev)))
		{
			ERR_error_string_n(oslerr, reps->body, sizeof(reps->body));
#endif
			fprintf(stderr, "%s\n", reps->body);
			printed_err = 1;
		}
		/* If the OpenSSL error queue was empty, maybe it was a
		 * socket error; let's try printing that. */
		if (!printed_err)
			fprintf(stderr, "socket error = %s (%d)\n",
					evutil_socket_error_to_string(reps->error_code),
					reps->error_code);
		return;
	}
/*
	fprintf(stderr, "Response line: %d %s\n",evhttp_request_get_response_code(req),evhttp_request_get_response_code_line(req));
	reps->error_code = evhttp_request_get_response_code(req);
*/
	sprintf(reps->reason,"%d %s",evhttp_request_get_response_code(req),evhttp_request_get_response_code_line(req));
	while ((reps->lenght = evbuffer_remove(evhttp_request_get_input_buffer(req),
										  reps->body, sizeof(reps->body))) > 0)
	{
		/* These are just arbitrary chunks of 256 bytes.
		 * They are not lines, so we can't treat them as such. */
		fwrite(reps->body, reps->lenght, 1, stdout);
	}

	
}


static void
err(const char *msg)
{
	fputs(msg, stderr);
}

#ifdef USE_MBEDTLS
static void
err_mbedtls(const char *func, int err)
{
	char buf[1024];
	mbedtls_strerror(err, buf, sizeof(buf));
	fprintf(stderr, "%s failed:%d, %s\n", func, err, buf);

	exit(1);
}

static int cert_verify_callback(void *userdata, mbedtls_x509_crt *crt,
								int depth, uint32_t *flags)
{
	*flags = 0;
	return 0;
}
#else
static void
err_openssl(const char *func)
{
	fprintf(stderr, "%s failed:\n", func);

	/* This is the OpenSSL function that prints the contents of the
	 * error stack to the specified file handle. */
	ERR_print_errors_fp(stderr);

	exit(1);
}

/* See http://archives.seul.org/libevent/users/Jan-2013/msg00039.html */
static int cert_verify_callback(X509_STORE_CTX *x509_ctx, void *arg)
{
	char cert_str[256];
	const char *host = (const char *)arg;
	const char *res_str = "X509_verify_cert failed";
	HostnameValidationResult res = Error;

	/* This is the function that OpenSSL would call if we hadn't called
	 * SSL_CTX_set_cert_verify_callback().  Therefore, we are "wrapping"
	 * the default functionality, rather than replacing it. */
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
		res = validate_hostname(host, server_cert);

		switch (res)
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
					  cert_str, sizeof(cert_str));

	if (res == MatchFound)
	{
		printf("https server '%s' has this certificate, "
			   "which looks good to me:\n%s\n",
			   host, cert_str);
		return 1;
	}
	else
	{
		printf("Got '%s' for hostname '%s' and certificate:\n%s\n",
			   res_str, host, cert_str);
		return 0;
	}
}
#endif

#if defined(_WIN32) && !defined(USE_MBEDTLS)
static int
add_cert_for_store(X509_STORE *store, const char *name)
{
	HCERTSTORE sys_store = NULL;
	PCCERT_CONTEXT ctx = NULL;
	int r = 0;

	sys_store = CertOpenSystemStore(0, name);
	if (!sys_store)
	{
		err("failed to open system certificate store\n");
		return -1;
	}
	while ((ctx = CertEnumCertificatesInStore(sys_store, ctx)))
	{
		X509 *x509 = d2i_X509(NULL, (unsigned char const **)&ctx->pbCertEncoded,
							  ctx->cbCertEncoded);
		if (x509)
		{
			X509_STORE_add_cert(store, x509);
			X509_free(x509);
		}
		else
		{
			r = -1;
			err_openssl("d2i_X509");
			break;
		}
	}
	CertCloseStore(sys_store, 0);
	return r;
}
#endif

int https_client(https_client_t *client, const char *url, int timeout)
{
	int r;
	
	const char *data_file = NULL;
	const char *crt = NULL;
	const char *scheme, *host, *path, *query;
	char uri[256];
	int port;
	int retries = 0;
	if (timeout == 0)
		timeout = -1;

#ifdef USE_MBEDTLS
	mbedtls_ssl_context ssl;
	mbedtls_ssl_config config;
	mbedtls_ctr_drbg_context ctr_drbg;
	mbedtls_entropy_context entropy;
	mbedtls_x509_crt cacert;
#else
	SSL_CTX *ssl_ctx = NULL;
	SSL *ssl = NULL;
#endif
	// struct bufferevent *bev;
	
	int ret = 0;

#ifdef USE_MBEDTLS
	mbedtls_x509_crt_init(&cacert);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);
	mbedtls_ssl_config_init(&config);
	mbedtls_ssl_init(&ssl);
#else
	enum
	{
		HTTP,
		HTTPS
	} type = HTTP;
#endif

	client->http_uri = evhttp_uri_parse(url);
	if (client->http_uri == NULL)
	{
		err("malformed url\n");
		goto error;
	}

	scheme = evhttp_uri_get_scheme(client->http_uri);
	if (scheme == NULL || (strcasecmp(scheme, "https") != 0 &&
						   strcasecmp(scheme, "http") != 0))
	{
		err("url must be http or https\n");
		goto error;
	}

	host = evhttp_uri_get_host(client->http_uri);
	if (host == NULL)
	{
		err("url must have a host\n");
		goto error;
	}

	port = evhttp_uri_get_port(client->http_uri);
	if (port == -1)
	{
		port = (strcasecmp(scheme, "http") == 0) ? 80 : 443;
	}

	path = evhttp_uri_get_path(client->http_uri);
	if (strlen(path) == 0)
	{
		path = "/";
	}

	query = evhttp_uri_get_query(client->http_uri);
	if (query == NULL)
	{
		snprintf(uri, sizeof(uri) - 1, "%s", path);
	}
	else
	{
		snprintf(uri, sizeof(uri) - 1, "%s?%s", path, query);
	}
	uri[sizeof(uri) - 1] = '\0';

#ifdef USE_MBEDTLS
	mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, (const unsigned char *)"libevent", sizeof("libevent"));
	mbedtls_ssl_config_defaults(&config, MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM, MBEDTLS_SSL_PRESET_DEFAULT);
	mbedtls_ssl_conf_rng(&config, mbedtls_ctr_drbg_random, &ctr_drbg);

	if (crt == NULL)
	{
		/* mbedtls has no function to read system CA certificates.
		 * so if there is no crt, we skip cert verify
		 */
		mbedtls_ssl_conf_verify(&config, cert_verify_callback, NULL);
		mbedtls_ssl_conf_ca_chain(&config, &cacert, NULL);
	}
	else
	{
		r = mbedtls_x509_crt_parse_file(&cacert, crt);
		if (r != 0)
		{
			err_mbedtls("mbedtls_x509_crt_parse_file", r);
			goto error;
		}
		mbedtls_ssl_conf_ca_chain(&config, &cacert, NULL);
	}

	mbedtls_ssl_setup(&ssl, &config);
#else
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
		err_openssl("RAND_poll");
		goto error;
	}

	/* Create a new OpenSSL context */
	ssl_ctx = SSL_CTX_new(SSLv23_method());
	if (!ssl_ctx)
	{
		err_openssl("SSL_CTX_new");
		goto error;
	}

	if (crt == NULL)
	{
		X509_STORE *store;
		/* Attempt to use the system's trusted root certificates. */
		store = SSL_CTX_get_cert_store(ssl_ctx);
#ifdef _WIN32
		if (add_cert_for_store(store, "CA") < 0 ||
			add_cert_for_store(store, "AuthRoot") < 0 ||
			add_cert_for_store(store, "ROOT") < 0)
		{
			goto error;
		}
#else  // _WIN32
		if (X509_STORE_set_default_paths(store) != 1)
		{
			err_openssl("X509_STORE_set_default_paths");
			goto error;
		}
#endif // _WIN32
	}
	else
	{
		if (SSL_CTX_load_verify_locations(ssl_ctx, crt, NULL) != 1)
		{
			err_openssl("SSL_CTX_load_verify_locations");
			goto error;
		}
	}
	/* Ask OpenSSL to verify the server certificate.  Note that this
	 * does NOT include verifying that the hostname is correct.
	 * So, by itself, this means anyone with any legitimate
	 * CA-issued certificate for any website, can impersonate any
	 * other website in the world.  This is not good.  See "The
	 * Most Dangerous Code in the World" article at
	 * https://crypto.stanford.edu/~dabo/pubs/abstracts/ssl-client-bugs.html
	 */
	SSL_CTX_set_verify(ssl_ctx, SSL_VERIFY_PEER, NULL);
	/* This is how we solve the problem mentioned in the previous
	 * comment.  We "wrap" OpenSSL's validation routine in our
	 * own routine, which also validates the hostname by calling
	 * the code provided by iSECPartners.  Note that even though
	 * the "Everything You've Always Wanted to Know About
	 * Certificate Validation With OpenSSL (But Were Afraid to
	 * Ask)" paper from iSECPartners says very explicitly not to
	 * call SSL_CTX_set_cert_verify_callback (at the bottom of
	 * page 2), what we're doing here is safe because our
	 * cert_verify_callback() calls X509_verify_cert(), which is
	 * OpenSSL's built-in routine which would have been called if
	 * we hadn't set the callback.  Therefore, we're just
	 * "wrapping" OpenSSL's routine, not replacing it. */
	SSL_CTX_set_cert_verify_callback(ssl_ctx, cert_verify_callback,
									 (void *)host);
#endif

	// Create event base
	client->base = event_base_new();
	if (!client->base)
	{
		perror("event_base_new()");
		goto error;
	}

#ifdef USE_MBEDTLS
	mbedtls_ssl_set_hostname(&ssl, host);
#else
	// Create OpenSSL bufferevent and stack evhttp on top of it
	ssl = SSL_new(ssl_ctx);
	if (ssl == NULL)
	{
		err_openssl("SSL_new()");
		goto error;
	}

#ifdef SSL_CTRL_SET_TLSEXT_HOSTNAME
	// Set hostname for SNI extension
	SSL_set_tlsext_host_name(ssl, host);
#endif
#endif

	if (strcasecmp(scheme, "http") == 0)
	{
		client->bev = bufferevent_socket_new(client->base, -1, BEV_OPT_CLOSE_ON_FREE);
	}
	else
	{
#ifdef USE_MBEDTLS
		bev = bufferevent_mbedtls_socket_new(base, -1, &ssl,
											 BUFFEREVENT_SSL_CONNECTING,
											 BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
#else
		type = HTTPS;
		client->bev = bufferevent_openssl_socket_new(client->base, -1, ssl,
													 BUFFEREVENT_SSL_CONNECTING,
													 BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
#endif
	}

	if (client->bev == NULL)
	{
		fprintf(stderr, "bufferevent_openssl_socket_new() failed\n");
		goto error;
	}

#ifdef USE_MBEDTLS
	bufferevent_mbedtls_set_allow_dirty_shutdown(client->bev, 1);
#else
	bufferevent_openssl_set_allow_dirty_shutdown(client->bev, 1);
#endif

	// For simplicity, we let DNS resolution block. Everything else should be
	// asynchronous though.
	client->evcon = evhttp_connection_base_bufferevent_new(client->base, NULL, client->bev,
												   host, port);
	if (client->evcon == NULL)
	{
		fprintf(stderr, "evhttp_connection_base_bufferevent_new() failed\n");
		goto error;
	}

	if (ipv4)
	{
		evhttp_connection_set_family(client->evcon, AF_INET);
	}
	if (ipv6)
	{
		evhttp_connection_set_family(client->evcon, AF_INET6);
	}

	if (retries > 0)
	{
		evhttp_connection_set_retries(client->evcon, retries);
	}
	if (timeout >= 0)
	{
		evhttp_connection_set_timeout(client->evcon, timeout);
	}

	// Fire off the request
	client->req = evhttp_request_new(http_request_done, client);
	if (client->req == NULL)
	{
		fprintf(stderr, "evhttp_request_new() failed\n");
		goto error;
	}

	client->output_headers = evhttp_request_get_output_headers(client->req);
	evhttp_add_header(client->output_headers, "Host", host);
	evhttp_add_header(client->output_headers, "Accept", "application/json");
	evhttp_add_header(client->output_headers, "User-Agent", "https://zh.wikipedia.org/wiki/User_agent");
	evhttp_add_header(client->output_headers, "Connection", "close");

	if (data_file)
	{
		/* NOTE: In production code, you'd probably want to use
		 * evbuffer_add_file() or evbuffer_add_file_segment(), to
		 * avoid needless copying. */
		FILE *f = fopen(data_file, "rb");
		char buf[1024];
		size_t s;
		size_t bytes = 0;

		if (!f)
		{
			goto error;
		}

		client->output_buffer = evhttp_request_get_output_buffer(client->req);
		while ((s = fread(buf, 1, sizeof(buf), f)) > 0)
		{
			evbuffer_add(client->output_buffer, buf, s);
			bytes += s;
		}
		evutil_snprintf(buf, sizeof(buf) - 1, "%lu", (unsigned long)bytes);
		evhttp_add_header(client->output_headers, "Content-Length", buf);
		fclose(f);
	}

	r = evhttp_make_request(client->evcon, client->req, data_file ? EVHTTP_REQ_POST : EVHTTP_REQ_GET, uri);
	if (r != 0)
	{
		fprintf(stderr, "evhttp_make_request() failed\n");
		goto error;
	}

	event_base_dispatch(client->base);
	goto cleanup;

error:
	ret = 1;
cleanup:
	if (client->evcon)
		evhttp_connection_free(client->evcon);
	if (client->http_uri)
		evhttp_uri_free(client->http_uri);
	if (client->base)
		event_base_free(client->base);

#ifdef USE_MBEDTLS
	mbedtls_ssl_free(&ssl);
	mbedtls_ssl_config_free(&config);
	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_x509_crt_free(&cacert);
#else
	if (ssl_ctx)
		SSL_CTX_free(ssl_ctx);
	if (type == HTTP && ssl)
		SSL_free(ssl);
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
#endif

	return ret;
}
