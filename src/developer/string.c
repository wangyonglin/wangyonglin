#include <wangyonglin/string.h>
#include <wangyonglin/url.h>
void *string_create(char **pointer, char *data, size_t data_size)
{
    if ((*pointer) = (char *)global_hooks.allocate(sizeof(char *) * data_size + 1))
    {
        memset((*pointer), 0x00, sizeof(char *) * data_size + 1);
        memcpy((*pointer), data, data_size);
        return (*pointer);
    }
    return NULL;
}

void string_delete(char *pointer)
{
    if (pointer)
    {
        free(pointer);
    }
}

void *object_create(void **pointer, size_t pointer_size)
{
    if ((*pointer) = global_hooks.allocate(pointer_size))
    {
        memset((*pointer), 0x00, pointer_size);
        return (*pointer);
    }
    return NULL;
}
void object_delete(void *pointer)
{
    if (pointer)
        free(pointer);
}

void boolean_create(unsigned char **pointer, boolean value)
{
    *pointer = value;
}
void integer_create(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}

char *string_localtime_create(char **pointer)
{
    if (!((*pointer) = malloc(sizeof(char) * 20)))
    {
        return NULL;
    }
    memset((*pointer), 0x00, sizeof(char) * 20);
    time_t rawtime;
    struct tm *info;
    time(&rawtime);
    info = gmtime(&rawtime);
    sprintf((*pointer), "%d-%d-%dT%d:%d:%dZ", info->tm_year + 1900, info->tm_mon, info->tm_mday, info->tm_hour, info->tm_min, info->tm_sec);
    return (*pointer);
}

char *string_url_encode(char *data, size_t data_size, char **pointer, size_t pointer_max_size)
{
    string_create(pointer, "", pointer_max_size);
    //  UrlEncode(data, *pointer, 1024);
    urlencode(data, data_size, *pointer, pointer_max_size);
    return (*pointer);
}
unsigned char uchar2hex(unsigned char x)
{
    return (unsigned char)(x > 9 ? x + 55 : x + 48);
}

static int is_alpha_number_char(unsigned char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        return 1;
    return 0;
}

// url编码实现

void urlencode(unsigned char *src, int src_len, unsigned char *dest, int dest_len)
{
    unsigned char ch;
    int len = 0;

    while (len < (dest_len - 4) && *src)
    {
        ch = (unsigned char)*src;
        if (*src == ' ')
        {
            *dest++ = '%';
            *dest++ = '2';
            *dest++ = '0';
        }
        else if (is_alpha_number_char(ch) || strchr("-_.!~*'()", ch))
        {
            *dest++ = *src;
        }
        else if (ch == '/')
        {
            *dest++ = '%';
            *dest++ = '2';
            *dest++ = '5';
            *dest++ = '2';
            *dest++ = 'F';
        }
        else
        {
            *dest++ = '%';
            *dest++ = uchar2hex((unsigned char)(ch >> 4));
            *dest++ = uchar2hex((unsigned char)(ch % 16));
        }
        ++src;
        ++len;
    }
    *dest = 0;
    return;
}

// 解url编码实现

unsigned char *urldecode(unsigned char *encd, unsigned char *decd)
{
    int j, i;
    char *cd = encd;
    char p[2];
    unsigned int num;
    j = 0;

    for (i = 0; i < strlen(cd); i++)
    {
        memset(p, '\0', 2);
        if (cd[i] != '%')
        {
            decd[j++] = cd[i];
            continue;
        }

        p[0] = cd[++i];
        p[1] = cd[++i];

        p[0] = p[0] - 48 - ((p[0] >= 'A') ? 7 : 0) - ((p[0] >= 'a') ? 32 : 0);
        p[1] = p[1] - 48 - ((p[1] >= 'A') ? 7 : 0) - ((p[1] >= 'a') ? 32 : 0);
        decd[j++] = (unsigned char)(p[0] * 16 + p[1]);
    }
    decd[j] = '\0';

    return decd;
}