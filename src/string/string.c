#include <string.h>
#include <ctype.h>

#include <string/string.h>


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
void string_del(char *str)
{
    if (str)
    {
        free(str);
        str = NULL;
    }
}
int string_ciphertext(char **dest, char *buffer, int length)
{

    return 0;
}
char *string_crt(char **str, char *dat, int len)
{
    if ((*str) = (char *)global_hooks.allocate(len))
    {
        bzero(*str, len);
        if (memcpy(*str, dat, len))
        {
            return *str;
        }
        global_hooks.deallocate(*str);
    }

    return NULL;
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
