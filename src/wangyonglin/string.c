#include <wangyonglin/string.h>

boolean string_is_null(string_t text)
{
    if (text.outstring)
        return negative;
    else
        return positive;
}
boolean string_is_empty(string_t text)
{
    if (text.outstring)
    {
        if (strcmp(text.outstring, "") == 0)
            return positive;
    }
    return negative;
}

void boolean_create(unsigned char **pointer, boolean value)
{
    *pointer = (unsigned char *)value;
}
void integer_create(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}

void message(const char *title, char *data)
{
    printf("\t%s:[%d]\t%s\r\n", title, strlen(data), data);
}

char *string_replace(char *formerString, size_t formerStringMax, const char findString, char *replaceString)
{

    char *tmpString = global_hooks.allocate(sizeof(char) * formerStringMax);
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
   // memset(formerString, 0x00, formerStringMax);
    if (formerString)
    {
        // formerString = realloc(formerString, j++);
        memset(formerString, 0x00, formerStringMax);
        memcpy(formerString, tmpString, j);
    }

    global_hooks.deallocate(tmpString);
    return formerString;
}

string_t string_create(char *datastring, size_t datalength)
{
    string_t out = string_null_command;
    if (datastring)
    {

        if ((out.outstring = global_hooks.allocate(datalength + 1)))
        {
            memset(out.outstring, 0x00, datalength + 1);
            memcpy(out.outstring, datastring, datalength);
            out.outlength = strlen(out.outstring);
            return out;
        }
    }
    return out;
}
void string_delete(string_t text)
{
    if (text.outstring)
        global_hooks.deallocate(text.outstring);
    text.outlength = 0;
    text.outstring = NULL;
}
void string_print(const char *title, string_t text)
{
    printf("\t%s:[%d]\t%s\r\n", title, text.outlength, text.outstring);
}