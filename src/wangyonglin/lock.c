#include <wangyonglin/lock.h>
#include <wangyonglin/buffer.h>
int _fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}
boolean locked(struct _lock_t *lock)
{

    if (!lock)
    {
        return ArgumentException;
    }
    if (lock->lockfd < 0)
    {
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if (fcntl(lock->lockfd, F_SETLK, &fl) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                fprintf(stdout, "alone runnind");
                return positive;
            }
            else
            {
                return negative;
            }
        }
    }
    return ErrorException;
}

lock_t *lock_create(lock_t **lock, pool_t *pool, conf_t *conf)
{

    if (pool)
    {
        if (pool_object_create(pool, (void **)lock, sizeof(lock_t)))
        {
            if (conf->lockfile[0] == '/')
            {
                pool_buffer_create(pool,  &(*lock)->lockfile, conf->lockfile, strlen(conf->lockfile));
            }
            else
            {
                size_t lockfilesize = 0;
                lockfilesize += strlen(PACKAGE_DIRECTERY_PREFIX);
                lockfilesize += strlen(conf->lockfile);
                lockfilesize += 2;
                char tmpLockfile[lockfilesize];
                memset(tmpLockfile, 0x00, sizeof(tmpLockfile));
                strcat(tmpLockfile, PACKAGE_DIRECTERY_PREFIX);
                strcat(tmpLockfile, "/");
                strcat(tmpLockfile, conf->lockfile);
                pool_buffer_create(pool, &(*lock)->lockfile, tmpLockfile, strlen(tmpLockfile));
            }
            return (*lock);
        }
    }

    return NULL;
}

ok_t locking(struct _lock_t *lock)
{
    if (!lock || !lock->lockfile)
    {
        fprintf(stderr, "lock ArgumentException");
        return ArgumentException;
    }

    if ((lock->lockfd = open(lock->lockfile, O_RDWR | O_CREAT, 0666)) < 0)
    {
        fprintf(stderr, "unable to open file '%s': %s", lock->lockfile, strerror(errno));
        return NullPointerException;
    }

    if (_fcntl(lock->lockfd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            perror("alone runnind");
            exit(EXIT_SUCCESS);
        }
        fprintf(stderr, "can't lock %s: %s", lock->lockfile, strerror(errno));
    }
    char buf[16];
    ftruncate(lock->lockfd, 0); // 设置文件的大小为0
    sprintf(buf, "%ld", (long)getpid());
    write(lock->lockfd, buf, strlen(buf) + 1);
    return OK;
}

ok_t unlocking(struct _lock_t *lock)
{
    if (!lock && !lock->lockfile)
    {
        return ArgumentException;
    }

    if (lock->lockfd != -1)
    {
        close(lock->lockfd);
    }
    if (unlink(lock->lockfile) != 0)
    {
        fprintf(stderr, "delect failed:[%s] %s ", lock->lockfile, strerror(errno));
    }

    return OK;
}
ok_t lockexit(struct _lock_t *lock)
{
    if (!lock && !lock->lockfile)
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
    if ((fd = open(lock->lockfile, O_RDWR, 0666)) != -1)
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
                        if (unlink(lock->lockfile) != 0)
                        {
                            fprintf(stderr, "delect failed:[%s] %s ", lock->lockfile, strerror(errno));
                        }
                    }
                    return OK;
                }
            }
        }
        close(fd);
    }

    return NoneException;
}
