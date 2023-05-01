#include <Stringex.h>
#include <SnowFlake.h>

void *ObjectCreate(void **deststring, size_t destlength)
{

    if (((*deststring) = global_hooks.allocate(destlength)))
    {
        memset((*deststring), 0x00, destlength);
        return (*deststring);
    }
    return (*deststring) = NULL;
}
void ObjectDelete(void *deststring)
{
    if (deststring)
    {
        global_hooks.deallocate(deststring);
    }
}
char *StringReallocate(char **deststring, char *valuestring)
{
    if (valuestring)
    {
        size_t valuelength = strlen(valuestring);
        if ((*deststring) = global_hooks.reallocate((*deststring), valuestring + 1))
        {
            memset((*deststring), 0x00, valuestring + 1);
            memcpy((*deststring), valuestring, valuelength);
            return (*deststring);
        }
    }
    return NULL;
}
/**
 * @brief
 *
 * @param deststring
 * @param valuestring
 * @param valuelength
 * @return char*
 */
char *StringCreate(char **deststring, char *valuestring, size_t valuelength)
{
    if (valuelength > 0)
    {
        if (((*deststring) = global_hooks.allocate(valuelength + 1)))
        {
            memset((*deststring), 0x00, valuelength + 1);
            if (valuestring)
            {
                memcpy((*deststring), valuestring, valuelength);
            }

            return (*deststring);
        }
    }
    return NULL;
}

/**
 * @brief
 *
 * @param deststring
 * @param valuestring
 * @param valuelength
 * @return char*
 */
char *StringResetting(char **deststring, char *valuestring, size_t valuelength)
{
    if (valuestring && valuelength > 0)
    {

        if ((*deststring) = global_hooks.reallocate((*deststring), valuelength + 1))
        {
            memset((*deststring), 0x00, valuelength + 1);
            memcpy((*deststring), valuestring, valuelength);
            return (*deststring);
        }
    }
    return NULL;
}
/**
 * @brief
 *
 * @param datastring
 */
void StringDelete(char *datastring)
{
    if (datastring)
        free(datastring);
}

/**
 * @brief
 *
 * @param deststring
 * @param datastring
 * @param datasize
 * @return char*
 */
char *StringexCreate(Stringex *deststring, char *valuestring, size_t valuelength)
{
    deststring->valuelength = 0;
    deststring->valuestring = NULL;
    if (valuestring && valuelength > 0)
    {
        if ((deststring->valuestring = global_hooks.allocate(valuelength + 1)))
        {
            memset(deststring->valuestring, 0x00, valuelength + 1);
            memcpy(deststring->valuestring, valuestring, valuelength);
            deststring->valuelength = valuelength;
            return deststring->valuestring;
        }
    }
    return NULL;
}
/**
 * @brief
 *
 * @param deststring
 */
void StringexDelete(Stringex deststring)
{
    if (deststring.valuestring)
        global_hooks.deallocate(deststring.valuestring);
    deststring.valuelength = 0;
    deststring.valuestring = NULL;
}

void hex_print(const void *pv, size_t len)
{
    printf("\t========%d========\r\n", len);
    const unsigned char *p = (const unsigned char *)pv;
    if (NULL == pv)
    {
        printf("NULL");
    }
    else
    {
        size_t i = 0;
        for (; i < len; ++i)
        {
            printf("%02X ", *p++);
        }
    }
    printf("\n");
}
char *StringexCat(Stringex *datastring, char *valulestring, size_t valuelength)
{

    if (strncat(datastring->valuestring, valulestring, valuelength))
    {
        datastring->valuelength += valuelength;
    }
    return NULL;
}
char *StringexSlice(Stringex *datastring, Stringex deststring, size_t spos, size_t epos)
{
    datastring->valuestring = NULL;
    datastring->valuelength = 0;
    if (deststring.valuestring && spos >= 0 && epos > spos)
    {
        size_t datamaxsize = epos - spos + 1;
        char tmpstring[datamaxsize];
        memset(tmpstring, 0x00, sizeof(tmpstring));
        size_t tmplength = 0;

        if (spos < deststring.valuelength && epos <= deststring.valuelength)
        {
            for (size_t i = 0; i < deststring.valuelength; i++)
            {
                if (spos <= i && epos > i)
                {
                    tmpstring[tmplength++] = deststring.valuestring[i];
                }
            }
            tmpstring[tmplength] = '\0';
        }
        return StringexCreate(datastring, tmpstring, tmplength);
    }
    return datastring->valuestring;
}

Boolean Stringex_IsString(Stringex deststring)
{
    if (deststring.valuestring)
    {
        return Boolean_true;
    }
    return Boolean_false;
}
Boolean Stringex_IsEmpty(Stringex deststring)
{

    if (Stringex_IsString(deststring))
    {
        if (strcmp(deststring.valuestring, "") != 0)
        {
            return Boolean_true;
        }
    }

    return Boolean_false;
}
void StringexResetting(Stringex *datastring, char *valuestring, size_t valuelength)
{
    if (valuestring && valuelength > 0)
    {

        if (datastring->valuestring = global_hooks.reallocate(datastring->valuestring, valuelength + 1))
        {
            memset(datastring->valuestring, 0x00, valuelength + 1);
            memcpy(datastring->valuestring, valuestring, valuelength);
            datastring->valuestring[valuelength]='\0';
            datastring->valuelength = valuelength;
        }
    }
}
Boolean StringexId(Stringex *id)
{
    char toString[32];
    bzero(toString, sizeof(toString));
    int64_t result = SnowFlake_IdGenerator();
    sprintf(toString, "%ld", result);
    if (StringexCreate(id, toString, strlen(toString)))
    {
        return Boolean_true;
    }
    return Boolean_false;
}

Boolean StringexNonce(Stringex *noce, size_t bits)
{
    if (bits >= 2)
    {
        char toString[bits];
        bzero(toString, sizeof(toString));
        int i, j;
        unsigned char str[2 + 1] = {0};
        const char *hex_digits16 = "0123456789ABCDEF";
        const char *hex_digits8 = "01234567";
        const char *hex_digits4 = "0123";
        const char *hex_digits2 = "01";
        srand(time(NULL));

        for (j = 0; j < bits / 2; j++)
        {
            str[1] = hex_digits16[rand() % 16];
            str[0] = hex_digits4[rand() % 4];
            strcat(toString, str);
        }
        if (StringexCreate(noce, toString, strlen(toString)))
        {
            return Boolean_true;
        }
    }

    return Boolean_false;
}

Boolean StringexTimeUTC(Stringex *utc)
{
    char tmpstring[80];
    memset(tmpstring, 0x00, sizeof(tmpstring));
    UtcTime result;
    time_t rawtime;
    time(&rawtime);
    result = unix32_to_UTC(rawtime);
    sprintf(tmpstring, "%04d-%02d-%02dT%02d:%02d:%02dZ", result.year, result.month, result.day,
            result.hour, result.minute, result.second); // 以年月日_时分秒的形式表示当前时间
    if (StringexCreate(utc, tmpstring, strlen(tmpstring)))
    {
        return Boolean_true;
    }
    return Boolean_false;
}

Boolean StringexTimestamp(Stringex *timestamp)
{

    char toString[16] = {0};
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // long result = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    // printf("second:%ld\n", tv.tv_sec); // 秒
    // printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000); // 毫秒
    // printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec); // 微秒
    sprintf(toString, "%ld", tv.tv_sec);
    if (StringexCreate(timestamp, toString, strlen(toString)))
    {
        return Boolean_true;
    }
    return Boolean_false;
}