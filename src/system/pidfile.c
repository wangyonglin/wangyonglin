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
#include <system/string.h>
#include <inttypes.h>
#include <system/log.h>
#include <system/pidfile.h>
#include <system/error.h>

int config_lock_fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}


ok_t system_pidfile_initializing(allocate_pool_t *pool, const char *filename, config_pidfile_t **pidfile)
{
    if (pool)
    {

        if ((*pidfile) = system_allocate_create(pool, sizeof(config_pidfile_t)))
        {
            (*pidfile)->name = system_allocate_create(pool, 64);
            bzero((*pidfile)->name, 64);
            sprintf((*pidfile)->name, filename);
            return ok;
        }
    }
    return ok_err_failure;
}
flag_t system_pidfile_listene(config_pidfile_t *pidfile)
{
    config_fd_t fd;
    if (pidfile->name)
    {
        if ((fd = open(pidfile->name, O_RDWR | O_CREAT, 0666)) < 0)
        {
            err_printf("unable to open file '%s': %s", pidfile->name, strerror(errno));
            return FLAG_ERROR;
        }

        if (config_lock_fcntl(fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                close(fd);
                err_printf("alone runnind");
                pidfile->alone_runnind = TRUE;
                return FLAG_TRUE;
            }
            err_printf("can't lock %s: %s", pidfile->name, strerror(errno));
        }
        close(fd);
    }
    return FLAG_FALSE;
}

ok_t system_pidfile_crt(config_pidfile_t *pidfile)
{
    if (pidfile)
    {
        char buf[16];
        if ((pidfile->fd = open(pidfile->name, O_RDWR | O_CREAT, 0666)) < 0)
        {
            err_printf("unable to open file '%s': %s", pidfile->name, strerror(errno));
            return ok_err_failure;
        }

        if (config_lock_fcntl(pidfile->fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                err_printf("alone runnind");
                pidfile->alone_runnind = TRUE;
                return ok_err_onlock;
            }
            err_printf("can't lock %s: %s", pidfile->name, strerror(errno));
        }

        ftruncate(pidfile->fd, 0); //设置文件的大小为0
        sprintf(buf, "%ld", (long)getpid());
        write(pidfile->fd, buf, strlen(buf) + 1);
        return ok;
    }
    return ok_err_failure;
}

ok_t system_pidfile_del(config_pidfile_t *pidfile)
{
    if (pidfile)
    {       
        if (pidfile->fd != -1)
        {
            close(pidfile->fd);
        }
    }
}
void system_pidfile_kill(config_pidfile_t *pidfile)
{
    if (pidfile->name)
    {
        int fd;
        int pid;
        char buf[16] = {0};
        struct flock fl;
        fl.l_type = F_WRLCK;
        fl.l_start = 0;
        fl.l_whence = SEEK_SET;
        fl.l_len = 0;
        if ((fd = open(pidfile->name, O_RDWR, 0666)) != -1)
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
                            if (unlink(pidfile->name) != 0)
                            {
                                err_printf("delect failed:[%s] %s ", pidfile->name, strerror(errno));
                            }
                        }
                    }
                }
            }
            close(fd);
        }
    }
}
/*
config_lock_t *config_lock_crt(config_lock_t **lock, const char *filename)
{
    if (allocate((void **)lock, sizeof(config_lock_t)))
    {
        string_crt(&((*lock)->name), strdup(filename), strlen(filename));
        (*lock)->onlock = FALSE;
        char buf[16];
        (*lock)->fd = open((*lock)->name, O_RDWR | O_CREAT, 0666);
        if ((*lock)->fd < 0)
        {
            err_printf("unable to open file '%s': %s", (*lock)->name, strerror(errno));
            exit(1);
        }
        if (config_lock_fcntl((*lock)->fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                close((*lock)->fd);
                string_del((*lock)->name);
                err_printf("alone runnind");
                (*lock)->onlock = TRUE;
                return NULL;
            }
            err_printf("can't lock %s: %s", (*lock)->name, strerror(errno));
        }
        else
        {
            (*lock)->onlock = FALSE;
        }
        ftruncate((*lock)->fd, 0); //设置文件的大小为0
        sprintf(buf, "%ld", (long)getpid());
        write((*lock)->fd, buf, strlen(buf) + 1);
        return (*lock);
    }
    return NULL;
}

void config_lock_del(config_lock_t *lock)
{
    if (lock->fd != -1)
    {
        close(lock->fd);
    }
    string_del(lock->name);
    deallocate(lock);
}

ok_t config_lock_unlink(config_lock_t *lock)
{

    if (unlink(lock->name) != 0)
    {
        err_printf("delect failed:[%s] %s ", lock->name, strerror(errno));
        return ERROR;
    }
    return OK;
}

ok_t config_lock_kill(config_lock_t *lock)
{
    int fd;
    int pid;
    char buf[16] = {0};
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    if ((fd = open(lock->name, O_RDWR, 0666)) != -1)
    {
        if (fcntl(fd, F_SETLK, &fl) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                if (read(fd, buf, sizeof(buf)) != -1)
                {
                    kill(atoi(buf), SIGTERM);
                    close(fd);
                    return OK;
                }
            }
        }
        close(fd);
        err_printf("config_lock_kill failed:[%s] %s ", lock->name, strerror(errno));
    }
    return ERROR;
}
*/