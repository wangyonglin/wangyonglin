#if !defined(INCLUDE_WANGYONGLIN_STRING_H)
#define INCLUDE_WANGYONGLIN_STRING_H
#include <wangyonglin/package.h>
#define string_null_command  {0,NULL}

typedef struct _string_t{
    size_t outlength;
    char * outstring;
}string_t;


void boolean_create(unsigned char **pointer, boolean value);
void integer_create(unsigned char **pointer, long value);

void message(const char *title, char *data);

char *string_replace(char *formerString, size_t formerStringMax, const char findString, char *replaceString);

boolean string_is_null(string_t text);
boolean string_is_empty(string_t text);

string_t string_create(char *datastring, size_t datalength);
void string_delete(string_t text);
void string_print(const char * title,string_t text);
#endif