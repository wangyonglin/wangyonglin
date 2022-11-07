
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <errno.h>

#define NTHREADS 5

typedef struct httpd_info
{
	struct event_base *base;
	struct evhttp *httpd;
} httpd_info;

typedef struct
{
	pthread_t ths[NTHREADS];
	httpd_info info_arr[NTHREADS];
	httpd_info *pinfo;
} app_pthread_t;
int main(int argv, char *args[])
{

	pthread_t ths[NTHREADS];
	httpd_info info_arr[NTHREADS], *pinfo;
	int i, ret, opt = 1, server_socket;
	server_socket = bind_socket();

	for (i = 0; i < NTHREADS; i++)
	{
		pinfo = &info_arr[i];
		pinfo->base = event_base_new();
		if (pinfo->base == NULL)
			errx(-1, "ERROR new base\n");
		pinfo->httpd = evhttp_new(pinfo->base);
		if (pinfo->httpd == NULL)
			errx(-1, "ERROR new evhttp\n");
		ret = evhttp_accept_socket(pinfo->httpd, server_socket);
		if (ret != 0)
			errx(-1, "Error evhttp_accept_socket\n");

		evhttp_set_cb(pinfo->httpd, "/testing", testing, 0);
		evhttp_set_gencb(pinfo->httpd, notfound, 0);
		ret = pthread_create(&ths[i], NULL, dispatch, pinfo);
	}
	for (i = 0; i < NTHREADS; i++)
	{
		pthread_join(ths[i], NULL);
	}

	return EXIT_SUCCESS;
}