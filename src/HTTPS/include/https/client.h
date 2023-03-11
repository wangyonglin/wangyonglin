#if !defined(INCLUDE_HTTPS_CLIENT_H)
#define INCLUDE_HTTPS_CLIENT_H
#include <stdlib.h>

typedef struct _https_callback
{
    size_t result;
    char prompt[2048];
    char jsonformat[2048];
    struct bufferevent *bev;
} https_callback;

int https_client(char *url, char *data_file, struct _https_callback **response);
#endif
