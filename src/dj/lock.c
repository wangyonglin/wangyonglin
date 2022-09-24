#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <signal.h>
#include <dj/string.h>
#include <inttypes.h>
#include <dj/log.h>
#include <dj/lock.h>
#include <dj/error.h>

int dj_lock_fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}
dj_int_t dj_lock_exist(dj_lock_t *lock, dj_log_t *log)
{

    char buf[16];
    lock->fd = open(lock->name, O_RDWR | O_CREAT, 0666);
    if (lock->fd < 0)
    {
        dj_log_error(log, "unable to open file '%s': %s", lock->name, strerror(errno));
        exit(1);
    }
    if (dj_lock_fcntl(lock->fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(lock->fd);
            dj_log_error(log, "alone runnind");
            return DJ_LOCK_EXIST;
        }
        dj_log_error("can't lock %s: %s", lock->name, strerror(errno));
    }
    ftruncate(lock->fd, 0); //设置文件的大小为0
    sprintf(buf, "%ld", (long)getpid());
    write(lock->fd, buf, strlen(buf) + 1);
    return DJ_LOCK_OK;
}

dj_int_t dj_lock_init(dj_lock_t **lock, dj_log_t *log, const char *name)
{
    if ((*lock) = (dj_lock_t *)global_hooks.allocate(sizeof(dj_lock_t)))
    {
        string_crt(&((*lock)->name), name, strlen(name));
        return DJ_LOCK_OK;
    }
    dj_log_error(log, "dj_lock_init malloc failed:[%s]", name);
    return DJ_LOCK_ERROR;
}

void dj_lock_del(dj_lock_t *lock)
{
    if (lock->fd != -1)
    {
        close(lock->fd);
    }
}
void dj_lock_clean(dj_lock_t *lock)
{
    if (lock)
    {
        string_del(lock->name);
        global_hooks.deallocate(lock);
    }
}

dj_int_t dj_unlink(const char *name)
{

    if (unlink(name) != 0)
    {
        dj_debug("delect failed:[%s] %s ", name, strerror(errno));
        return DJ_LOCK_ERROR;
    }
    return DJ_LOCK_OK;
}

dj_int_t dj_kill(const char *name)
{
    int fd;
    int pid;
    char buf[16] = {0};
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    if ((fd = open(name, O_RDWR, 0666)) != -1)
    {
        if (fcntl(fd, F_SETLK, &fl) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                if (read(fd, buf, sizeof(buf)) != -1)
                {
                    kill(atoi(buf), SIGTERM);
                    close(fd);         
                    return DJ_LOCK_OK;
                }
            }
        }
        dj_debug("kill failed:[%s] %s ", name, strerror(errno));
        close(fd);
    }
    return DJ_LOCK_ERROR;
}