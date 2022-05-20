
#include <wangyonglin/linux.h>

#include <encryption/Base64.h>
#include <encryption/Utils.h>
#define KEY_LENGTH 24
#define SEPARATOR "$"
#define SALTLEN 12

int utils_timestamp(char **dest)
{

    char buffer[10];
    bzero(buffer, sizeof(buffer));
    int retlen = 0;
    time_t seconds = time(NULL); // The function time(NULL) returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
    if (sprintf(buffer, "%-llu", (unsigned long long int)seconds) > 0)
    {
        retlen = strlen(buffer);
        if (*dest == NULL)
        {
            *dest = (char *)malloc(retlen + 1);
            if (*dest == NULL)
                return -2;
        }
        bzero(*dest, sizeof(*dest));
        memcpy(*dest, buffer, strlen(buffer));
        return 0;
    }
    return -1;
}

int utils_randhex(char **dest, int bits)
{
    BIGNUM *bn;
    bn = BN_new(); //生成一个BIGNUM结构
    int top = -1;
    int bottom = 1;
    BN_rand(bn, bits, top, bottom); //生成指定bits的随机数
    char *buffer = BN_bn2hex(bn);   //转化成16进制字符串
    if (*dest == NULL)
    {
        *dest = (char *)malloc(strlen(buffer) + 1);
        if (*dest == NULL)
            return -2;
    }
    bzero(*dest, sizeof(*dest));
    memcpy(*dest, buffer, strlen(buffer));
    BN_free(bn); //释放BIGNUM结构
    return 0;
}

int utils_ciphertext(char **dest, char *buffer, int length)
{
   
    return 0;
}