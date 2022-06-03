#if !defined(__STRING_STRING_H__)
#define __STRING_STRING_H__
#include <wangyonglin/linux.h>
#include <string/Base64.h>
#include <string/SHA256WithRSA.h>

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
char *string_timestamp_crt(char **dest);
char *string_rand_crt(char **dest, int bits);

#endif //!__STRING_STRING_H__