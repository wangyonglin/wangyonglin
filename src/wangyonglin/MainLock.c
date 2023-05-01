#include <MainLock.h>
#include <unistd.h>
#include <sys/types.h>
int _fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}


Boolean MainLockCreate(MainLock **mainLock)
{
    Boolean err = Boolean_false;
    if (ObjectCreate((void **)mainLock, sizeof(MainLock)))
    {
        memset((*mainLock), 0x00, sizeof(MainLock));
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
        StringexCreate(&(*mainLock)->pidFileName, tmpString, strlen(tmpString));
        return Boolean_true;
    }
    return err;
}

void MainLockDelete(MainLock *mainLock)
{

    if (mainLock)
    {
        StringexDelete(mainLock->pidFileName);
        ObjectDelete(mainLock);
    }
}

Boolean MainLockInit(MainLock *mainLock, zlog_category_t *log)
{
    Boolean err = Boolean_false;
    if (!mainLock)
        return err;
    if (!log)
        return err;
    if (!Stringex_IsString(mainLock->pidFileName) && !Stringex_IsEmpty(mainLock->pidFileName))
    {
        zlog_error(log, "进程序锁文件不能为空");
        return err;
    }
    if ((mainLock->fd = open(mainLock->pidFileName.valuestring, O_RDWR | O_CREAT, 0666)) < 0)
    {
        zlog_error(log, "%s 不能正常打开文件 %s", mainLock->pidFileName.valuestring, strerror(errno));
        return err;
    }

    if (_fcntl(mainLock->fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            zlog_info(log, "程序已经运行");
            exit(EXIT_SUCCESS);
        }
    }
    char buf[16];
    ftruncate(mainLock->fd, 0); // 设置文件的大小为0
    sprintf(buf, "%ld", (long)getpid());
    write(mainLock->fd, buf, strlen(buf) + 1);
    return Boolean_true;
}

void MainLockUnlink(MainLock *mainLock)
{
    if (mainLock)
    {
        if (mainLock->fd != -1)
        {
            close(mainLock->fd);
        }
        if (unlink(mainLock->pidFileName.valuestring) != 0)
        {
            // logerr(lock->log, "delect failed:[%s] %s ", lock->pid.valuestring, strerror(errno));
        }
    }
}

void MainLockExit(MainLock *mainLock)
{
    if (mainLock)
    {
        int fd;
        int pid;
        char buf[16] = {0};
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if ((fd = open(mainLock->pidFileName.valuestring, O_RDWR, 0666)) != -1)
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
                            if (unlink(mainLock->pidFileName.valuestring) != 0)
                            {
                                // logerr(lock->log, "delect failed:[%s] %s ", lock->pid.valuestring, strerror(errno));
                            }
                        }
                        return;
                    }
                }
            }
            close(fd);
        }
    }

  
}
