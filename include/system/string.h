#if !defined(__SYSTEM_STRING_H__)
#define __SYSTEM_STRING_H__
#include <stdio.h>
#include <malloc.h>
#include <system/types.h>
#include <system/pool.h>
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

int *string_cmp(const char *__s1, const char *__s2);
char *string_crt(char **pointer, char *data, int length);
char *string_upa(char **pointer, char *data, int length);
void string_del(char *dest);

void *allocate_object(void **pointer, size_t size);
void deallocate_object(void *pointer);

void *allocate(void **pointer, size_t size);
void deallocate(void *pointer);


char *setting_string(char *pointer, const char *data, size_t length);
int format_string(char *pointer, const char *fmt, ...) __attribute__((format(printf, 2, 3)));


#endif //!__WANGYONGLIN_STRING_H__