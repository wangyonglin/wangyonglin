#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <openssl/bn.h>
#include <skinny.h>
int skinny_timestamp(char **dest)
{
    *dest = (char *)malloc(sizeof(char *) * 12);
    bzero(*dest, sizeof(*dest));
    time_t seconds = time(NULL); // The function time(NULL) returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.
    sprintf(*dest, "%-llu", (unsigned long long int)seconds);
    return 0;
}

int skinny_hexrand(char **dest, int bits)
{
    *dest = (char *)malloc(bits / 4 + 1);
    (*dest)[bits / 4 + 1] = '\0';
    BIGNUM *bn;
    bn = BN_new(); //生成一个BIGNUM结构
    int top = -1;
    int bottom = 1;
    BN_rand(bn, bits, top, bottom); //生成指定bits的随机数
    *dest = BN_bn2hex(bn);          //转化成16进制字符串
    BN_free(bn);                    //释放BIGNUM结构
    return 0;
}