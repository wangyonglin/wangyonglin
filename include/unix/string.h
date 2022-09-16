#if !defined(__UNIX_STRING_H__)
#define __UNIX_STRING_H__
#include <stdio.h>
#include <malloc.h>
typedef struct
{
    void *(*allocate)(size_t size);
    void (*deallocate)(void *pointer);
    void *(*reallocate)(void *pointer, size_t size);
} internal_hooks_t;

static internal_hooks_t global_hooks = {malloc, free, realloc};
//int utils_ciphertext(char **dest, char *buffer, int length);
char *string_crt(char **str, char *dat, int len);
void string_del(char*dest);
int string_len(const char *str);
char *string_timestamp_crt(char **dest);
char *string_rand_crt(char **dest, int bits);
unsigned char *string_hex_bytes(unsigned char **bytes, const char *string,int length);


#endif //!__WANGYONGLIN_STRING_H__