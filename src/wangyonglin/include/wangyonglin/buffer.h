#if !defined(INCLUDE_WANGYONGLIN_BUFFER_H)
#define INCLUDE_WANGYONGLIN_BUFFER_H
#include <wangyonglin/package.h>

boolean buffer_is_null(char *outbuffer);
boolean buffer_is_empty(char *outbuffer);
char *buffer_create(char **outstring, char *datastring, size_t datalength);
void buffer_delete(char *datastring);
void buffer_print(const char *title, char *text);
char *buffer_max_create(char **formerString, size_t formerStringMax, char *datastring, size_t datalength);
#endif