#if !defined(INCLUDE_WANGYONGLIN_STRING_H)
#define INCLUDE_WANGYONGLIN_STRING_H
#include <wangyonglin/package.h>

void boolean_create(unsigned char **pointer, boolean value);
void integer_create(unsigned char **pointer, long value);

void *objcrt(void **obj, size_t objsize);
void objdel(void *obj);

void string_rows(const char *title, char *data);
char *string_replace(char *formerString, size_t formerStringMax, const char *findString, char *replaceString);

char *strcrt(char **obj, char *dat, size_t datsz);
char *strnull(char **obj, size_t objmaxsz);
void strdel(char *obj);

void *memcrt(void **obj, size_t objsz, void *dat, size_t datsz);
void *memnull(void **obj, size_t objsz);
void memdel(void *obj);

#endif