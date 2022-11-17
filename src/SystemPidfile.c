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
#include <SystemString.h>
#include <inttypes.h>
#include <SystemLog.h>
#include <SystemPidfile.h>
#include <SystemError.h>

ConfUtils_command_t SystemPidfile_commands[] = {
    {"pid_file", string, offsetof(SystemPidfile_t, name)}};

int SystemPidfile_commands_size = sizeof(SystemPidfile_commands) / sizeof(SystemPidfile_commands[0]);

int config_lock_fcntl(int fd)
{
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return (fcntl(fd, F_SETLK, &fl));
}

ok_t SystemPidfile_initializing(SystemPidfile_t **SystemPidfile, AllocateUtils_t *AllocateUtils, ConfUtils_t *ConfUtils)
{

    if (!AllocateUtils && !ConfUtils)
    {
        return ArgumentException;
    }

    if (!((*SystemPidfile) = AllocateUtils_pool(AllocateUtils, sizeof(SystemPidfile_t))))
    {
        return NullPointerException;
    }
    ConfUtils_final(ConfUtils, (void **)SystemPidfile, sizeof(SystemPidfile_t), NULL, SystemPidfile_commands, SystemPidfile_commands_size);
    (*SystemPidfile)->AllocateUtils = AllocateUtils;
    return OK;
}
ok_t SystemPidfile_listene(SystemPidfile_t *SystemPidfile)
{
    if (!SystemPidfile && SystemPidfile->name)
    {
        return ArgumentException;
    }
    config_fd_t fd;
    if ((fd = open(SystemPidfile->name, O_RDWR | O_CREAT, 0666)) < 0)
    {
        SystemError_Message("unable to open file '%s': %s", SystemPidfile->name, strerror(errno));

        return ErrorException;
    }

    if (config_lock_fcntl(fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            close(fd);
            SystemError_Message("alone runnind");
            SystemPidfile->alone_runnind = TRUE;
            return NoneException;
        }
        SystemError_exitMessage(SystemPidfile->AllocateUtils, "can't lock %s: %s", SystemPidfile->name, strerror(errno));
    }
    close(fd);

    return OK;
}

ok_t SystemPidfile_crt(SystemPidfile_t *SystemPidfile)
{
    if (!SystemPidfile && SystemPidfile->name)
    {
        return ArgumentException;
    }

    char buf[16];
    if ((SystemPidfile->fd = open(SystemPidfile->name, O_RDWR | O_CREAT, 0666)) < 0)
    {
        SystemError_exitMessage(SystemPidfile->AllocateUtils, "unable to open file '%s': %s", SystemPidfile->name, strerror(errno));
        return NullPointerException;
    }

    if (config_lock_fcntl(SystemPidfile->fd) < 0)
    {
        if (errno == EACCES || errno == EAGAIN)
        {
            SystemError_Message("alone runnind");
            SystemPidfile->alone_runnind = TRUE;
            return NoneException;
        }
        SystemError_exitMessage(SystemPidfile->AllocateUtils, "can't lock %s: %s", SystemPidfile->name, strerror(errno));
    }

    ftruncate(SystemPidfile->fd, 0); // 设置文件的大小为0
    sprintf(buf, "%ld", (long)getpid());
    write(SystemPidfile->fd, buf, strlen(buf) + 1);
    return OK;

    return ErrorException;
}

ok_t SystemPidfile_del(SystemPidfile_t *SystemPidfile)
{
    if (!SystemPidfile && !SystemPidfile->name)
    {
        return ArgumentException;
    }

    if (SystemPidfile->fd != -1)
    {
        close(SystemPidfile->fd);
    }
    if (unlink(SystemPidfile->name) != 0)
    {
        SystemError_exitMessage(SystemPidfile->AllocateUtils, "delect failed:[%s] %s ", SystemPidfile->name, strerror(errno));
    }

    return OK;
}
ok_t SystemPidfile_quit(SystemPidfile_t *SystemPidfile)
{
    if (!SystemPidfile && !SystemPidfile->name)
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
    if ((fd = open(SystemPidfile->name, O_RDWR, 0666)) != -1)
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
                        if (unlink(SystemPidfile->name) != 0)
                        {
                            SystemError_exitMessage(SystemPidfile->AllocateUtils, "delect failed:[%s] %s ", SystemPidfile->name, strerror(errno));
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