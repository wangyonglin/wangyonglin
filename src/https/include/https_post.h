#if !defined(INCLUDE_HTTPS_POST_H)
#define INCLUDE_HTTPS_POST_H
#include <https_result.h>

long https_post(https_result *result, const char *url, char *postdata, size_t postdatasize);
#endif
