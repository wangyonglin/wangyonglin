#include <wangyonglin/lock.h>
#include <wangyonglin/buffer.h>
#include <wangyonglin/object.h>
#include <wangyonglin/log.h>
int _fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}
boolean locked(lock_t *lock)
{

    if (lock->fd < 0)
    {
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if (fcntl(lock->fd, F_SETLK, &fl) < 0)
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

lock_t *lock_create(lock_t **lock, log_t *log)
{
    if (object_create((void **)lock, sizeof(lock_t)))
    {
        size_t maxsize = 0;
        (*lock)->piddir = datasheet_create(PACKAGE_DIRECTERY_LOCALDIR, strlen(PACKAGE_DIRECTERY_LOCALDIR));
        if (datasheet_value((*lock)->piddir))
        {
            mkdir(datasheet_value((*lock)->piddir), 0755);
            maxsize += datasheet_length((*lock)->piddir);
            maxsize += sizeof(PACKAGE_NAME);
            maxsize++;
            maxsize += sizeof(".pid");
            maxsize++;
            char tmpString[maxsize];
            memset(&tmpString, 0x00, sizeof(tmpString));
            strcat(tmpString, datasheet_value((*lock)->piddir));
            strcat(tmpString, "/");
            strcat(tmpString, PACKAGE_NAME);
            strcat(tmpString, ".pid");
            (*lock)->pidfile = datasheet_create(tmpString, strlen(tmpString));
            (*lock)->log = log;
            return (*lock);
        }
        object_delete((*lock));
    }
    return NULL;
}

void lock_delete(lock_t *lock)
{

    if (lock)
    {
        datasheet_delete(lock->piddir);
        datasheet_delete(lock->pidfile);
        object_delete(lock);
    }
}

ok_t locking(lock_t *lock)
{
    if (lock && datasheet_value(lock->pidfile))
    {

        if ((lock->fd = open(datasheet_value(lock->pidfile), O_RDWR | O_CREAT, 0666)) < 0)
        {
            //  fprintf(stderr, "unable to open file '%s': %s", lock->lockfile, strerror(errno));
            logerr(lock->log, "unable to open file '%s': %s", datasheet_value(lock->pidfile), strerror(errno));
            return NullPointerException;
        }

        if (_fcntl(lock->fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                loginfo(lock->log, "alone runnind");
                exit(EXIT_SUCCESS);
            }
            //  fprintf(stderr, "can't lock %s: %s", lock->lockfile, strerror(errno));
            logerr(lock->log, "can't lock %s: %s", datasheet_value(lock->pidfile), strerror(errno));
        }
        char buf[16];
        ftruncate(lock->fd, 0); // 设置文件的大小为0
        sprintf(buf, "%ld", (long)getpid());
        write(lock->fd, buf, strlen(buf) + 1);
    }

    return OK;
}

void unlocking(lock_t *lock)
{
    if (lock && datasheet_value(lock->pidfile))
    {
        if (lock->fd != -1)
        {
            close(lock->fd);
        }
        if (unlink(datasheet_value(lock->pidfile)) != 0)
        {
            logerr(lock->log, "delect failed:[%s] %s ", datasheet_value(lock->pidfile), strerror(errno));
        }
    }
}

ok_t lockexit(lock_t *lock)
{
    if (lock && datasheet_value(lock->pidfile))
    {
        int fd;
        int pid;
        char buf[16] = {0};
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if ((fd = open(datasheet_value(lock->pidfile), O_RDWR, 0666)) != -1)
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
                            if (unlink(datasheet_value(lock->pidfile)) != 0)
                            {
                                logerr(lock->log, "delect failed:[%s] %s ", datasheet_value(lock->pidfile), strerror(errno));
                            }
                        }
                        return OK;
                    }
                }
            }
            close(fd);
        }
    }

    return NoneException;
}
