#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdarg.h>
#include <system/string.h>
#include <openssl/bn.h>

#define KEY_LENGTH 24
#define SEPARATOR "$"
#define SALTLEN 12

char *string_timestamp_crt(char **dest)
{
    char buffer[10] = {0};
    time_t seconds = time(NULL); // The function time(NULL) returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
    if (sprintf(buffer, "%-llu", (unsigned long long int)seconds) > 0)
    {
        int len = strlen(buffer);
        if ((*dest = (char *)global_hooks.allocate(len + 1)))
        {
            bzero(*dest, sizeof(*dest));
            memcpy(*dest, buffer, len);
        }
    }
    return *dest;
}

char *string_rand_crt(char **dest, int bits)
{
    if (bits < 0 && bits > 2048)
        return NULL;
    BIGNUM *bn = NULL;
    char *buffer = NULL;
    //生成一个BIGNUM结构
    if ((bn = BN_new()))
    { //生成指定bits的随机数
        BN_rand(bn, bits * 4, -1, 1);
        //转化成16进制字符串
        if ((buffer = BN_bn2hex(bn)))
        {
            if ((*dest = (char *)global_hooks.allocate(strlen(buffer) + 1)))
            {
                bzero(*dest, sizeof(*dest));
                memcpy(*dest, buffer, strlen(buffer));
            }
        }
        BN_free(bn); //释放BIGNUM结构
        bn = NULL;
    }

    return *dest;
}
int string_len(const char *str)
{
    return strlen(str);
}

int string_ciphertext(char **dest, char *buffer, int length)
{

    return 0;
}

unsigned char *string_hex_bytes(unsigned char **bytes, const char *string, int length)

{

    if (((*bytes) = (unsigned char *)global_hooks.allocate(length + 1)))
    {
        bzero((*bytes), length + 1);
        unsigned char highByte, lowByte;

        for (int i = 0; i < length; i += 2)
        {
            //转换成大写字母
            highByte = toupper(string[i]);
            lowByte = toupper(string[i + 1]);
            //转换编码
            if (highByte > 0x39)
            {
                highByte -= 0x37;
            }
            else
            {
                highByte -= 0x30;
            }
            if (lowByte > 0x39)
            {
                lowByte -= 0x37;
            }
            else
            {
                lowByte -= 0x30;
            }
            //高4位和低4位合并成一个字节
            (*bytes)[i / 2] = (highByte << 4) | lowByte;
        }
        return (*bytes);
    }

    return NULL;
}

char *string_crt(char **pointer, char *data, int length)
{
    if ((*pointer) = (char *)global_hooks.allocate(length + 1))
    {
        if (memset((*pointer), 0x00, length))
        {
            if (memcpy((*pointer), data, length))
            {
                return (*pointer);
            }
        }
        global_hooks.deallocate((*pointer));
    }
    return NULL;
}

char *string_upa(char **pointer, char *data, int length)
{
    if ((*pointer) = (char *)global_hooks.reallocate((*pointer), length + 1))
    {
        if (memset((*pointer), 0x00, length))
        {
            if (memcpy((*pointer), data, length))
            {
                return (*pointer);
            }
        }
        global_hooks.deallocate((*pointer));
    }
    return NULL;
}
void string_del(char *str)
{
    if (str != NULL)
    {
        free(str);
        str = NULL;
    }
}
void *allocate_object(void **pointer, size_t size)
{

    if (((*pointer) = malloc(size)) == NULL)
    {
        perror("allocate_object 申请对象内存失败");
        return NULL;
    }

    if (memset((*pointer), 0x00, size))
    {
        return (*pointer);
    }
    return NULL;
}
void deallocate_object(void *pointer)
{
    if (pointer != NULL)
    {
        free(pointer);
        pointer = NULL;
    }
}
void *allocate(void **pointer, size_t size)
{

    if (((*pointer) = malloc(size)) == NULL)
    {
        perror("allocate 申请对象内存失败");
        return NULL;
    }

    if (memset((*pointer), 0x00, size))
    {
        return (*pointer);
    }
    return NULL;
}
void deallocate(void *pointer)
{
    if (pointer != NULL)
    {
        free(pointer);
        pointer = NULL;
    }
}


char *allocate_string(char **pointer, size_t size)
{
    if (((*pointer) = (char *)malloc(sizeof(char) * size)) == NULL)
    {
        perror("string_allocate 申请字符串内存失败");
        return NULL;
    }

    if (memset((*pointer), 0x00, sizeof(char) * size))
    {
        return (*pointer);
    }
    return NULL;
}
int format_string(char *pointer, const char *fmt, ...)
{
    va_list va;
    int rc;
    va_start(va, fmt);
    rc = sprintf(pointer, fmt, va);
    va_end(va);
    return rc;
}
void deallocate_string(char *pointer)
{
    if (pointer != NULL)
    {
        free(pointer);
        pointer = NULL;
    }
}

char *setting_string(char *pointer, const char *data, size_t length)
{

    memset(pointer, 0x00, strlen(pointer));
    return strncpy(pointer, data, length);
}