#if !defined(HTTPS_URL_H)
#define HTTPS_URL_H
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
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
 * @brief URLDecode : decode the urlencoded str to base64 encoded string
 *
 * @param str:  the urlencoded string
 * @param strsz:  the str length (exclude the last \0)
 * @param result:  the result buffer
 * @param resultsz: the result buffer size(exclude the last \0)
 *
 * @return: >=0 represent the decoded result length
 *              <0 encode failure
 *
 * Note:
 * 1) to ensure the result buffer has enough space to contain the decoded string, we'd better
 *     to set resultsz to strsz
 *
 */
int URLDecode(const char *str, const int strsz, char *result, const int resultsz, const char **last_pos);

/**
 * 将字符串formerString中找到的每一个findString字符串替换为replaceString
 *主要思路：若无匹配字符串，返回原字符串。若有，新建一个指针指向关键字符串之后的字符串，截取关键字符串之前的字符串，与Replacement拼接，得到的结果再与关键字符串之后的字符串拼接
 *@param formerString 待查字符串
 *@param formerStringMax 待查字符串最大小
 *@param findString  关键字符串
 *@param replaceString  替换字符串
 *@return: 返回替换后的字符串头指针
 *
 */
char *URLReplace(char *formerString, size_t formerStringMax, const char findString, char *replaceString);
#endif