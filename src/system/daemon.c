#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <system/daemon.h>
#include <system/error.h>

int config_daemon_start()
{

    switch (fork())
    {
    case -1:
        err_printf("fork() failed");
        return -1;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        err_printf("could change to root dir");
        return -1;
    }
    if (setsid() == -1)
    {
        err_printf("\t\tsetsid() failed");
        return -1;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        err_printf("open(\"/dev/null\") failed");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        err_printf("dup2(STDIN) failed");
        return -1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        err_printf("dup2(STDOUT) failed");
        return -1;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            err_printf("close() failed");
            return -1;
        }
    }
    return 0;
}
