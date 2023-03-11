#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <Encrypt/URL.h>

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
			ch == '-' || ch == '_' || ch == '*' || ch == '~' || ch == '.')
			result[j++] = ch;
		else if (ch == ' ')
		{
			result[j++] = '+';
			// result[j++] = '%';
			// result[j++] = '2';
			// result[j++] = '0';
		}
		else if (ch == '+')
		{

			result[j++] = '%';
			result[j++] = '2';
			result[j++] = '0';
		}
		else
		{
			if (j + 3 <= resultsz)
			{
				if (ch == ':')
				{
					result[j++] = '%';
					result[j++] = '2';
					result[j++] = '5';
					result[j++] = hexchars[(unsigned char)ch >> 4];
					result[j++] = hexchars[(unsigned char)ch & 0xF];
				}
				else if (ch == '/')
				{
					result[j++] = '%';
					result[j++] = '2';
					result[j++] = '5';
					result[j++] = hexchars[(unsigned char)ch >> 4];
					result[j++] = hexchars[(unsigned char)ch & 0xF];
				}
				else
				{
					result[j++] = '%';
					result[j++] = hexchars[(unsigned char)ch >> 4];
					result[j++] = hexchars[(unsigned char)ch & 0xF];
				}
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
int URLDecode(const char *str, const int strsz, char *result, const int resultsz, const char **last_pos)
{
	int i, j;
	char ch;
	char a;

	*last_pos = str;
	if (strsz < 0 || resultsz < 0)
		return -1;

	for (i = 0, j = 0; i < strsz && j < resultsz; j++)
	{
		ch = *(str + i);

		if (ch == '+')
		{
			result[j] = ' ';
			i += 1;
		}
		else if (ch == '%')
		{
			if (i + 3 <= strsz)
			{
				ch = *(str + i + 1);

				if (ch >= 'A' && ch <= 'F')
				{
					a = (ch - 'A') + 10;
				}
				else if (ch >= '0' && ch <= '9')
				{
					a = ch - '0';
				}
				else if (ch >= 'a' && ch <= 'f')
				{
					a = (ch - 'a') + 10;
				}
				else
				{
					return -2;
				}

				a <<= 4;

				ch = *(str + i + 2);
				if (ch >= 'A' && ch <= 'F')
				{
					a |= (ch - 'A') + 10;
				}
				else if (ch >= '0' && ch <= '9')
				{
					a |= (ch - '0');
				}
				else if (ch >= 'a' && ch <= 'f')
				{
					a |= (ch - 'a') + 10;
				}
				else
				{
					return -2;
				}

				result[j] = a;

				i += 3;
			}
			else
				break;
		}
		else if ((ch >= 'A' && ch <= 'Z') ||
				 (ch >= 'a' && ch <= 'z') ||
				 (ch >= '0' && ch <= '9') ||
				 ch == '.' || ch == '-' || ch == '*' || ch == '_')
		{

			result[j] = ch;
			i += 1;
		}
		else
		{
			return -2;
		}
	}

	*last_pos = str + i;
	return j;
}

/**
 * @brief 字符串替换代码块 开始
 *
 */
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

char *URLReplace(char *formerString, size_t formerStringMax, const char findString, char *replaceString)
{
	char *tmpString = malloc(sizeof(char) * formerStringMax);
	memset(tmpString, 0x00, sizeof(char) * formerStringMax);
	int formerStringSize = strlen(formerString);
	int i, j;
	char ch;

	if (formerStringSize < 0)
		return NULL;

	for (i = 0, j = 0; i < formerStringSize; i++)
	{
		if (formerString[i] == findString)
		{
			for (size_t x = 0; x < strlen(replaceString); x++)
			{
				tmpString[j++] = replaceString[x];
			}
		}
		else
		{
			tmpString[j++] = formerString[i];
		}
	}
	if (!formerString)
	{
		formerString = malloc(sizeof(char) * formerStringMax);
	}

	memset(formerString, 0x00, sizeof(char) * formerStringMax);
	formerString = memcpy(formerString, tmpString, strlen(tmpString));
	free(tmpString);
	return formerString;
}
