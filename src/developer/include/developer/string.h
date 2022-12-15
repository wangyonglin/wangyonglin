#if !defined(DEVELOPER_STRING_H)
#define DEVELOPER_STRING_H
#include <unistd.h>
#include <stdio.h>
#include <getopt.h> //getopt_long
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <developer/allocate.h>
#include <developer/conf.h>


void boolean_setting(unsigned char **pointer, boolean value);
void integer_setting(unsigned char **pointer, long value);
void string_setting(unsigned char **pointer,char *value);

// void *allocate(void **pointer, size_t pointer_size);
// void deallocate(void *pointer);

typedef struct{
    char * name;
    void * data;
}item_t;

int items_by_name(const void *e1, const void *e2);
void items_sort(item_t *item,size_t item_count);



#endif