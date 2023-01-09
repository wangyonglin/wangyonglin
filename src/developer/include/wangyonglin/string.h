#if !defined(INCLUDE_WANGYONGLIN_STRING_H)
#define INCLUDE_WANGYONGLIN_STRING_H
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <stdbool.h>

typedef struct internal_hooks
{
    void *(*allocate)(size_t size);
    void (*deallocate)(void *pointer);
    void *(*reallocate)(void *pointer, size_t size);
} internal_hooks;
static internal_hooks global_hooks = {malloc, free, realloc};

typedef int ok_t;

#define OK ((ok_t)0)
#define DoneException ((ok_t)1)
#define NoneException ((ok_t)0)
#define NullPointerException ((ok_t)-1)
#define ErrorException ((ok_t)-2)
#define ArgumentException ((ok_t)-3)
#define UnknownException ((ok_t)-4)
#define FileNotFoundException ((ok_t)-5)
#define ExitException ((ok_t)-9)

typedef int string;
typedef int integer;
typedef int boolean;

#define positive ((integer)1)
#define negative ((integer)0)
#define invalid ((integer)-1)

#define STRING ((integer)1)
#define INTEGER ((integer)2)
#define BOOLEAN ((integer)3)

#define ENABLED ((boolean)1)
#define DISABLED ((boolean)0)

#define onstart ((integer)1)
#define onstop ((integer)0)
#define onstatus ((integer)-1)

void *string_create(char **pointer, char *data, size_t data_size);
void string_delete(char *pointer);
void boolean_create(unsigned char **pointer, boolean value);
void integer_create(unsigned char **pointer, long value);

void *object_create(void **pointer, size_t pointer_size);
void object_delete(void *pointer);

char *string_localtime_create(char **pointer);
char *string_url_encode(char *data, size_t data_size, char **pointer ,size_t pointer_max_size);
unsigned char uchar2hex(unsigned char x);
unsigned char *urldecode(unsigned char *encd, unsigned char *decd);
static int is_alpha_number_char(unsigned char c);
void urlencode(unsigned char *src, int src_len, unsigned char *dest, int dest_len);
#endif