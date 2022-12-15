#include <developer/pidlock.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <developer/allocate.h>
#include <developer/pidlock.h>

int config_lock_fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}
boolean pidlock_startused(pidlock_t *pidlock)
{

    if (!pidlock)
    {
        return ArgumentException;
    }
    if (pidlock->lockfd < 0)
    {
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if (fcntl(pidlock->lockfd, F_SETLK, &fl) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                fprintf(stdout, "alone runnind");
                return ENABLED;
            }
            else
            {
                return DISABLED;
            }
        }
    }
    return ErrorException;
};
ok_t pidlock_initializing(pidlock_t **pidlock, mapping_t *mapping, char *cnf)
{
    if (!mapping)
    {
        return ArgumentException;
    }
    if (object_create(mapping->allocate, (void **)pidlock, sizeof(pidlock_t)) != Ok)
    {
        return ErrorException;
    }
    mapping_arguments_t arguments[] = {
        {"pidfile", NULL, STRING, offsetof(pidlock_t, pidfile)}};
    int arguments_size = sizeof(arguments) / sizeof(arguments[0]);

    if (mapping_create(mapping, (*pidlock), cnf, NULL, arguments, arguments_size) == Ok)
    {
        (*pidlock)->mapping = mapping;
        return Ok;
    }
    return ErrorException;
}
ok_t pidlock_status(pidlock_t *pidlock)
{
    if (!pidlock && !pidlock->pidfile)
    {
        fprintf(stderr, "pidlock_status ArgumentException");
        return ArgumentException;
    }
    int fd;
    if ((fd = open(pidlock->pidfile, O_RDWR | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "unable to open file '%s': %s", pidlock->pidfile, strerror(errno));
        return ErrorException;
    }

    if (config_lock_fcntl(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            fprintf(stdout, "alone runnind");
            pidlock->statused = ENABLED;
            return Ok;
        }
        fprintf(stderr, "can't lock %s: %s", pidlock->pidfile, strerror(errno));
    }
    close(fd);

    return Ok;
}

ok_t pidlock_locking(pidlock_t *pidlock)
{
    if (!pidlock && !pidlock->pidfile)
    {
        fprintf(stderr, "pidlock_locking ArgumentException");
        return ArgumentException;
    }

    char buf[16];

    if ((pidlock->lockfd = open(pidlock->pidfile, O_RDWR | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "unable to open file '%s': %s", pidlock->pidfile, strerror(errno));
        return NullPointerException;
    }

    if (config_lock_fcntl(pidlock->lockfd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            fprintf(stdout, "alone runnind");
            pidlock->statused = ENABLED;
            return NoneException;
        }
        fprintf(stderr, "can't lock %s: %s", pidlock->pidfile, strerror(errno));
    }

    ftruncate(pidlock->lockfd, 0); // 设置文件的大小为0
    sprintf(buf, "%ld", (long)getpid());
    write(pidlock->lockfd, buf, strlen(buf) + 1);
    return Ok;
}

ok_t pidlock_unlocking(pidlock_t *pidlock)
{
    if (!pidlock && !pidlock->pidfile)
    {
        return ArgumentException;
    }

    if (pidlock->lockfd != -1)
    {
        close(pidlock->lockfd);
    }
    if (unlink(pidlock->pidfile) != 0)
    {
        fprintf(stderr, "delect failed:[%s] %s ", pidlock->pidfile, strerror(errno));
    }

    return Ok;
}
ok_t pidlock_exit(pidlock_t *pidlock)
{
    if (!pidlock && !pidlock->pidfile)
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
    if ((fd = open(pidlock->pidfile, O_RDWR, 0666)) != -1)
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
                        if (unlink(pidlock->pidfile) != 0)
                        {
                            fprintf(stderr, "delect failed:[%s] %s ", pidlock->pidfile, strerror(errno));
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
