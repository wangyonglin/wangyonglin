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

config_pidfile_t *config_pidfile_allocate(config_pidfile_t **pidfile)
{

    if (allocate_object((void **)pidfile, sizeof(config_pidfile_t)))
    {
        allocate_string(&((*pidfile)->name), 512);
        return (*pidfile);
    }
    return NULL;
}
void config_pidfile_deallocate(config_pidfile_t *pidfile)
{
    if (pidfile)
    {
        deallocate_string(pidfile->name);
        deallocate_object(pidfile);
    }
}
ok_t config_pidfile_initializing(config_pidfile_t *pidfile, const char *filename)
{
    if (pidfile && pidfile->name)
    {
        setting_string(pidfile->name, filename, strlen(filename));
        return OK_SUCCESS;
    }
    return OK_NONE;
}

flag_t config_pidfile_listene(config_pidfile_t *pidfile)
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

ok_t config_pidfile_crt(config_pidfile_t *pidfile)
{
    if (pidfile)
    {
        char buf[16];
        if ((pidfile->fd = open(pidfile->name, O_RDWR | O_CREAT, 0666)) < 0)
        {
            err_printf("unable to open file '%s': %s", pidfile->name, strerror(errno));
            return OK_ERROR;
        }

        if (config_lock_fcntl(pidfile->fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                err_printf("alone runnind");
                pidfile->alone_runnind = TRUE;
                return OK_LOCK;
            }
            err_printf("can't lock %s: %s", pidfile->name, strerror(errno));
        }

        ftruncate(pidfile->fd, 0); //设置文件的大小为0
        sprintf(buf, "%ld", (long)getpid());
        write(pidfile->fd, buf, strlen(buf) + 1);
        return OK_SUCCESS;
    }
    return OK_NONE;
}

ok_t config_pidfile_del(config_pidfile_t *pidfile)
{
    if (pidfile)
    {
        string_del(pidfile->name);
        if (pidfile->fd != -1)
        {
            close(pidfile->fd);
        }
    }
}
void config_pidfile_kill(config_pidfile_t *pidfile)
{
    if (pidfile)
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