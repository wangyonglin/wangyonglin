#if !defined(STRING_INCLUDE_BYTE_BY_THIS_H)
#define STRING_INCLUDE_BYTE_BY_THIS_H
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <string_by_this.h>
char *byte_copy(char *byte, char *datastring, size_t datasize);
char * byte_create(char ** byte,char * datastring,size_t datasize);
void byte_delete(char *byte);
char *byte_strcat(char *bytes, char *datastring);
#endif