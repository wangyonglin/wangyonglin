#include <string_by_hex.h>

char*  string_by_hex(string_by_t *data,size_t bits)
{
    if (bits >= 2)
    {
        char toString[bits];
        bzero(toString,sizeof(toString));
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
        string_create(data,toString, strlen(toString));
        return  *data->valuestring;
    }

    return NULL;
}