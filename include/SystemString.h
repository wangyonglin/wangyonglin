#if !defined(INCLUDE_SYSTEMSTRING_H)
#define INCLUDE_SYSTEMSTRING_H
#include <stdio.h>
#include <malloc.h>
#include <SystemTypes.h>
#include <SystemAllocate.h>
typedef struct
{
    void *(*allocate)(size_t size);
    void (*deallocate)(void *pointer);
    void *(*reallocate)(void *pointer, size_t size);
} internal_hooks_t;

static internal_hooks_t global_hooks = {malloc, free, realloc};
// int utils_ciphertext(char **dest, char *buffer, int length);

int string_len(const char *str);
char *string_timestamp_crt(char **dest);
char *string_rand_crt(char **dest, int bits);
unsigned char *string_hex_bytes(unsigned char **bytes, const char *string, int length);


char *SystemString_initializing(char **pointer, char *data, int length);
void SystemString_cleanup(char *pointer);


void *allocate(void **pointer, size_t size);
void deallocate(void *pointer);


char *setting_string(char *pointer, const char *data, size_t length);
int format_string(char *pointer, const char *fmt, ...) __attribute__((format(printf, 2, 3)));


#endif //!__WANGYONGLIN_STRING_H__