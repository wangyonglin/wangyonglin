#if !defined(WANGYONGLIN_PACKAGE_H)
#define WANGYONGLIN_PACKAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#include <string.h>
#include <config.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/bio.h>

#define datasheet_null_command {0,NULL}

typedef struct internal_hooks
{
    void *(*allocate)(size_t objsize);
    void (*deallocate)(void *obj);
    void *(*reallocate)(void *obj, size_t objsize);

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

typedef enum _type_t
{
    INVALID,
    STRING,
    INTEGER,
    BOOLEAN
} type_t;

typedef int boolean;
#define negative ((boolean)0)
#define positive ((boolean)1)
#define invalid ((boolean)-1)

typedef int integer;
#define onstart ((integer)1)
#define onstop ((integer)0)
#define onstatus ((integer)-1)

typedef struct _datasheet_t
{
    size_t length;
    char *string;
} datasheet;

// typedef struct _package_t
// {
//     datasheet logdir;
//     datasheet localdir;
//     datasheet confdir;
// } package;

// package *package_create(package **back);
// void package_delete(package *back);

// package *global_package;
datasheet datasheet_create(char *datastring, size_t datalength);
void datasheet_delete(datasheet data);
char *datasheet_value(datasheet data);
size_t datasheet_length(datasheet data);
#endif