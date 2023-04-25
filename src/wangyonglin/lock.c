#include <wangyonglin/lock.h>
#include <string_by_this.h>

int _fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}
boolean_by_t locked(lock_t *lock)
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

lock_t *lock_create(lock_t **lock)
{
    if (((*lock) = (lock_t *)global_hooks.allocate(sizeof(lock_t))))
    {
        memset((*lock), 0x00, sizeof(lock_t));
        size_t maxsize = 8;
        mkdir(PACKAGE_DIRECTERY_LOCALDIR, 0755);
        maxsize += sizeof(PACKAGE_DIRECTERY_LOCALDIR);
        maxsize += sizeof(PACKAGE_NAME);
        maxsize += sizeof(".pid");
        char tmpString[maxsize];
        memset(&tmpString, 0x00, sizeof(tmpString));
        strcat(tmpString, PACKAGE_DIRECTERY_LOCALDIR);
        strcat(tmpString, "/");
        strcat(tmpString, PACKAGE_NAME);
        strcat(tmpString, ".pid");
        string_create(&(*lock)->pid, tmpString, strlen(tmpString));
        return (*lock);

        global_hooks.deallocate((*lock));
    }
    return NULL;
}

void lock_delete(lock_t *lock)
{

    if (lock)
    {

        string_delete(lock->pid);
        global_hooks.deallocate(lock);
    }
}

ok_t locking(lock_t *lock)
{
    if (lock && lock->pid.valuestring)
    {

        if ((lock->fd = open(lock->pid.valuestring, O_RDWR | O_CREAT, 0666)) < 0)
        {
            // logerr(lock->log, "unable to open file '%s': %s", lock->pid.valuestring, strerror(errno));
            return NullPointerException;
        }

        if (_fcntl(lock->fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                //  logerr(lock->log, "alone runnind");
                exit(EXIT_SUCCESS);
            }
            //  fprintf(stderr, "can't lock %s: %s", lock->lockfile, strerror(errno));
            // logerr(lock->log, "can't lock %s: %s", lock->pid.valuestring, strerror(errno));
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
    if (lock && lock->pid.valuestring)
    {
        if (lock->fd != -1)
        {
            close(lock->fd);
        }
        if (unlink(lock->pid.valuestring) != 0)
        {
           // logerr(lock->log, "delect failed:[%s] %s ", lock->pid.valuestring, strerror(errno));
        }
    }
}

ok_t lockexit(lock_t *lock)
{
    if (lock && lock->pid.valuestring)
    {
        int fd;
        int pid;
        char buf[16] = {0};
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if ((fd = open(lock->pid.valuestring, O_RDWR, 0666)) != -1)
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
                            if (unlink(lock->pid.valuestring) != 0)
                            {
                               // logerr(lock->log, "delect failed:[%s] %s ", lock->pid.valuestring, strerror(errno));
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
