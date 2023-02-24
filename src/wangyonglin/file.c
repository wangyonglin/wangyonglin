#include <wangyonglin/file.h>

// ok_t file_create(allocate_t *allocate, file_t **file)
// {
//     if (!allocate)
//     {
//         return ArgumentException;
//     }
//     if ((*file))
//     {
//         return OK;
//     }
//     if (allocate_object_create(allocate, (void **)file, sizeof(file_t)) != OK)
//     {
//         return ErrorException;
//     }
//     return OK;
// }

// ok_t file_getdata(allocate_t *allocate, file_t *file, char *filename)
// {
//     if (!file && !allocate && !filename)
//     {
//         return ArgumentException;
//     }
//     int fd;
//     if ((fd = open(filename, O_RDONLY)) == -1)
//     {
//         return FileNotFoundException;
//     }

//     if (fstat(fd, &file->filestat) == -1)
//     {
//         close(fd);
//         return ArgumentException;
//     }
//     allocate_object_create(allocate, (void **)&(file->filedata), file->filestat.st_size + 1);
//     memset(file->filedata, 0x00, file->filestat.st_size + 1);
//     if (read(fd, file->filedata, file->filestat.st_size + 1) != file->filestat.st_size)
//     {
//         close(fd);
//         return ErrorException;
//     }
//     file->filedata_size = file->filestat.st_size;
//     close(fd);
//     return OK;
// }

// void file_destroy(allocate_t *allocate, file_t *file)
// {
//     if (allocate && file)
//     {
//         if (file->filedata)
//             allocate_object_delete(allocate, file->filedata);
//         allocate_object_delete(allocate, file);
//     }
// }