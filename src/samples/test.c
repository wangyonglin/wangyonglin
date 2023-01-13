#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

static unsigned char hexchars[] = "0123456789ABCDEF";
/**
 * @brief URLEncode : encode the base64 string "str"
 *
 * @param str:  the base64 encoded string
 * @param strsz:  the str length (exclude the last \0)
 * @param result:  the result buffer
 * @param resultsz: the result buffer size(exclude the last \0)
 *
 * @return: >=0 represent the encoded result length
 *              <0 encode failure
 *
 * Note:
 * 1) to ensure the result buffer has enough space to contain the encoded string, we'd better
 *     to set resultsz to 3*strsz
 *
 * 2) we don't check whether str has really been base64 encoded
 */
int URLEncode(const char *str, const int strsz, char *result, const int resultsz);

/**
 * @brief URLEncode : encode the base64 string "str"
 *
 * @param str:  the base64 encoded string
 * @param strsz:  the str length (exclude the last \0)
 * @param result:  the result buffer
 * @param resultsz: the result buffer size(exclude the last \0)
 *
 * @return: >=0 represent the encoded result length
 *              <0 encode failure
 *
 * Note:
 * 1) to ensure the result buffer has enough space to contain the encoded string, we'd better
 *     to set resultsz to 3*strsz
 *
 * 2) we don't check whether str has really been base64 encoded
 */
int URLEncode(const char *str, const int strsz, char *result, const int resultsz)
{
    int i, j;
    char ch;

    if (strsz < 0 || resultsz < 0)
        return -1;

    for (i = 0, j = 0; i < strsz && j < resultsz; i++)
    {
        ch = *(str + i);
        if ((ch >= 'A' && ch <= 'Z') ||
            (ch >= 'a' && ch <= 'z') ||
            (ch >= '0' && ch <= '9') ||
            ch == '.' || ch == '-' || ch == '*' || ch == '_')
            result[j++] = ch;
        else if (ch == ' ')
            result[j++] = '+';
        else
        {
            if (j + 3 <= resultsz)
            {
                result[j++] = '%';
                result[j++] = hexchars[(unsigned char)ch >> 4];
                result[j++] = hexchars[(unsigned char)ch & 0xF];
            }
            else
            {
                return -2;
            }
        }
    }

    if (i == 0)
        return 0;
    else if (i == strsz)
        return j;
    return -2;
}

// return < 0: represent failure
int main(int argc, char *argv[])
{
    int ret;
    char buf[1024] = "GET&%2F&AccessKeyIdLTAI5t9fk9zx771m7bwNjgeYActionPubFormatJSONMessageContentwangyonglinProductKeya170p0o3VKDRegionIdcn-shanghaiSignatureMethodHMAC-SHA1Signon1.0Timestamp2023-01-12T11";
    char result[1024 * 3];

    ret = URLEncode(buf, strlen(buf), result, 1024 * 3);

    printf("%s", result);

    return ret;
}