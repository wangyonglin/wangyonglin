#include "stdio.h"
#include "stdlib.h"
#include <stdint.h>
#include "string.h"
#include <ctype.h>

/**
 十六进制字符串转字节流
 @param hexString 待转换的十六进制字符串
 @param byteString 保存转换后的字节流
 */
void hexStrToByte(const char *hexString, unsigned char *byteString)
{
    int hexStrLen = strlen(hexString);
    unsigned char highByte, lowByte;

    for (int i = 0; i < hexStrLen; i += 2)
    {
        //转换成大写字母
        highByte = toupper(hexString[i]);
        lowByte = toupper(hexString[i + 1]);
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
        byteString[i / 2] = (highByte << 4) | lowByte;
    }
    return;
}
int main(int argc, const char *argv)
{
    char Hexstring[64] = "";
    char Binarystring[256] = {0};
    hexStrToByte(Hexstring, Binarystring);
    int i = 0;
    for (i = 0; i < strlen(Binarystring); i++)
    {
        printf("\t%02hhX\n", Binarystring[i]);
    }
    return 0;
}