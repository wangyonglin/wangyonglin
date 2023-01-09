#if !defined(DEVELOPER_FILE_H)
#define DEVELOPER_FILE_H
#include <unistd.h>
#include <stdio.h>
#include <getopt.h> //getopt_long
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <developer/allocate.h>
#include <wangyonglin/conf.h>


typedef struct
{
    struct stat filestat;
    char *filedata;
    size_t filedata_size;
} file_t;

ok_t file_create(allocate_t *allocate, file_t **file);
ok_t file_getdata(allocate_t *allocate, file_t *file, char *filename);
void file_destroy( allocate_t *allocate,file_t *file);
#endif