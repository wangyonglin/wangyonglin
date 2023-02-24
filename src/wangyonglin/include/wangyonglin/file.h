#if !defined(INCLUDE_WANGYONGLIN_FILE_H)
#define INCLUDE_WANGYONGLIN_FILE_H
#include <wangyonglin/package.h>
#include <wangyonglin/conf.h>


typedef struct _file_t
{
    struct stat filestat;
    char *filedata;
    size_t filedata_size;
} file_t;

// ok_t file_create(allocate_t *allocate, file_t **file);
// ok_t file_getdata(allocate_t *allocate, file_t *file, char *filename);
// void file_destroy( allocate_t *allocate,file_t *file);

typedef struct _filename{
    char * name;
}filename;


#endif