#if !defined(INCLUDE_STRINGTHIS_H)
#define INCLUDE_STRINGTHIS_H
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <stdarg.h>
#include <Unix_timestamp_converter.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/bio.h>
#include <openssl/err.h>

typedef enum ObjectType_e
{
    OBJECT_TYPE_INVALID,
    OBJECT_TYPE_NULL,
    OBJECT_TYPE_FILE,
    OBJECT_TYPE_STRING,
    OBJECT_TYPE_INTEGER,
    OBJECT_TYPE_BOOLEAN
} ObjectType,
    ObjectType_t;

#ifndef INTERNAL_HOOKS_THIS
#define INTERNAL_HOOKS_THIS
typedef struct internal_hooks
{
    void *(*allocate)(size_t objsize);
    void (*deallocate)(void *obj);
    void *(*reallocate)(void *obj, size_t objsize);

} internal_hooks;
static internal_hooks global_hooks = {malloc, free, realloc};
#endif
#define stringex_null_command \
    {                         \
        0, NULL               \
    }

typedef long Boolean;
typedef long Integer;
typedef struct _Stringex_t Stringex;

#define Stringex_null \
    (Stringex) { 0, NULL }
#define Boolean_true ((Boolean)1)
#define Boolean_false ((Boolean)0)
#define Boolean_invalid ((Boolean)-1)
typedef struct _Stringex_t
{
    size_t valuelength;
    char *valuestring;
} *pStringex;

void *ObjectCreate(void **deststring, size_t destlength);
void ObjectDelete(void *deststring);

char *StringReallocate(char **deststring, char *valuestring);
/**
 * @brief
 *
 * @param deststring
 * @param valuestring
 * @param valuelength
 * @return char*
 */
char *StringCreate(char **deststring, char *valuestring, size_t valuelength);
/**
 * @brief
 *
 * @param deststring
 * @param valuestring
 * @param valuelength
 * @return char*
 */
char *StringResetting(char **deststring, char *valuestring, size_t valuelength);
/**
 * @brief
 *
 * @param datastring
 */
void StringDelete(char *datastring);

/**
 * @brief 创建字符串对象
 *
 * @param string
 * @param datastring
 * @param datasize
 * @return char*
 */
char *StringexCreate(Stringex *deststring, char *datastring, size_t datasize);
/**
 * @brief 删除字符串对象的值和注销指针
 *
 * @param string
 */

void StringexDelete(Stringex deststring);
void StringexResetting(Stringex *datastring, char *valuestring, size_t valuelength);
Boolean Stringex_IsString(Stringex deststring);
Boolean Stringex_IsEmpty(Stringex deststring);
char *StringexCat(Stringex *datastring, char *valulestring, size_t valuelength);
char *StringexSlice(Stringex *datastring, Stringex deststring, size_t spos, size_t epos);

void hex_print(const void *pv, size_t len);

Boolean StringexId(Stringex *id);
Boolean StringexNonce(Stringex *noce, size_t bits);
Boolean StringexTimeUTC(Stringex *utc);
Boolean StringexTimestamp(Stringex *timestamp);

#endif