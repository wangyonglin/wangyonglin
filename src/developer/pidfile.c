#include <developer/pidfile.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <developer/allocate.h>
#include <developer/pidfile.h>

int config_lock_fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}

ok_t pidfile_initializing(pidfile_t **pidfile, allocate_t *allocate, const char *filename)
{

    if (!allocate && !filename)
    {
        return ArgumentException;
    }
    if (object_crt(allocate, pidfile, sizeof(pidfile_t)) != Ok)
    {

        return ErrorException;
    }

    if (string_crt(allocate, &(*pidfile)->filename, strdup(filename), strlen(filename)) != Ok)
    {
        return ErrorException;
    }
    (*pidfile)->allocate = allocate;
    return Ok;
}
ok_t pidfile_status(pidfile_t *pidfile)
{
    if (!pidfile && pidfile->filename)
    {
        return ArgumentException;
    }
    int fd;
    if ((fd = open(pidfile->filename, O_RDWR | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "unable to open file '%s': %s", pidfile->filename, strerror(errno));
        return ErrorException;
    }

    if (config_lock_fcntl(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            fprintf(stdout, "alone runnind");
            pidfile->statused = enabled;
            return NoneException;
        }
        fprintf(stderr, "can't lock %s: %s", pidfile->filename, strerror(errno));
    }
    close(fd);

    return Ok;
}

ok_t pidfile_locking(pidfile_t *pidfile)
{
    if (!pidfile && pidfile->filename)
    {
        return ArgumentException;
    }

    char buf[16];

    if ((pidfile->lockfd = open(pidfile->filename, O_RDWR | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "unable to open file '%s': %s", pidfile->filename, strerror(errno));
        return NullPointerException;
    }

    if (config_lock_fcntl(pidfile->lockfd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            fprintf(stdout, "alone runnind");
            pidfile->statused = enabled;
            return NoneException;
        }
        fprintf(stderr, "can't lock %s: %s", pidfile->filename, strerror(errno));
    }

    ftruncate(pidfile->lockfd, 0); // 设置文件的大小为0
    sprintf(buf, "%ld", (long)getpid());
    write(pidfile->lockfd, buf, strlen(buf) + 1);
    return Ok;

    return ErrorException;
}

ok_t pidfile_unlocking(pidfile_t *pidfile)
{
    if (!pidfile && !pidfile->filename)
    {
        return ArgumentException;
    }

    if (pidfile->lockfd != -1)
    {
        close(pidfile->lockfd);
    }
    if (unlink(pidfile->filename) != 0)
    {
        fprintf(stderr, "delect failed:[%s] %s ", pidfile->filename, strerror(errno));
    }

    return Ok;
}
ok_t pidfile_exit(pidfile_t *pidfile)
{
    if (!pidfile && !pidfile->filename)
    {
        return ArgumentException;
    }
    int fd;
    int pid;
    char buf[16] = {0};
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    if ((fd = open(pidfile->filename, O_RDWR, 0666)) != -1)
    {
        if (fcntl(fd, F_SETLK, &fl) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                if (read(fd, buf, sizeof(buf)) != -1)
                {
                    close(fd);
                    if (kill(atoi(buf), SIGTERM) == 0)
                    {
                        if (unlink(pidfile->filename) != 0)
                        {
                            fprintf(stderr, "delect failed:[%s] %s ", pidfile->filename, strerror(errno));
                        }
                    }
                    return Ok;
                }
            }
        }
        close(fd);
    }

    return NoneException;
}
