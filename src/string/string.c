
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
    if (len > 0 && len < 2048)
    {
        int datlen = (sizeof(char) * len);
        if ((*str) = (char *)global_hooks.allocate(datlen))
        {
            bzero(*str, datlen);
            if (memcpy(*str, dat, len))
            {
                return *str;
            }
            global_hooks.deallocate(*str);
        }
    }

    return NULL;
}