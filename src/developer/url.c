#include <wangyonglin/url.h>
int UrlEncode(const char *str, char *result, const int max_length)
{
    int i;
    int j = 0;
    char ch;

    int strSize = strlen(str);

    if ((str == NULL) || (result == NULL) || (strSize <= 0) || (max_length <= 0))
    {
        return 0;
    }

    for (i = 0; (i < strSize) && (j < max_length); ++i)
    {
        ch = str[i];
        if (((ch >= 'A') && (ch <= 'Z')) ||
            ((ch >= 'a') && (ch <= 'z')) ||
            ((ch >= '0') && (ch <= '9')))
        {
            result[j++] = ch;
        }
        else if (ch == '.' || ch == '-' || ch == '_' || ch == '*' || ch == '~')
        {
            result[j++] = ch;
        }
        else if (ch == ' ')
        {
            result[j++] = '%';
            result[j++] = '2';
            result[j++] = '0';
        }
        else if (ch == '/')
        {
            result[j++] = '%';
            result[j++] = '2';
            result[j++] = '5';
            result[j++] = '2';
            result[j++] = 'F';
        }

        else
        {
            if (j + 3 < max_length)
            {
                sprintf(result + j, "%%%02x", (unsigned char)ch);
                j += 3;
            }
            else
            {
                return 0;
            }
        }
    }

    result[j] = '\0';
    return j;
}
