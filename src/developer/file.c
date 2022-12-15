#include <developer/file.h>

ok_t file_create(allocate_t *allocate, file_t **file)
{
    if (!allocate)
    {
        return ArgumentException;
    }
    if ((*file))
    {
        return Ok;
    }
    if (object_create(allocate, (void **)file, sizeof(file_t)) != Ok)
    {
        return ErrorException;
    }
    return Ok;
}

ok_t file_getdata(allocate_t *allocate, file_t *file, char *filename)
{
    if (!file && !allocate && !filename)
    {
        return ArgumentException;
    }
    int fd;
    if ((fd = open(filename, O_RDONLY)) == -1)
    {
        return FileNotFoundException;
    }

    if (fstat(fd, &file->filestat) == -1)
    {
        close(fd);
        return ArgumentException;
    }
    object_create(allocate, (void **)&(file->filedata), file->filestat.st_size + 1);
    memset(file->filedata, 0x00, file->filestat.st_size + 1);
    if (read(fd, file->filedata, file->filestat.st_size + 1) != file->filestat.st_size)
    {
        close(fd);
        return ErrorException;
    }
    file->filedata_size = file->filestat.st_size;
    close(fd);
    return Ok;
}

void file_destroy(allocate_t *allocate, file_t *file)
{
    if (allocate && file)
    {
        if (file->filedata)
            object_delete(allocate, file->filedata);
        object_delete(allocate, file);
    }
}