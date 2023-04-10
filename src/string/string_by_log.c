#include <string_by_log.h>

bool string_by_log(const char *title, string_by_t data)
{
    printf("\t%s:[%d]\t%s\r\n", title, data.valuelength, data.valuestring);
}