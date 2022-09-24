#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dj/daemon.h>
#include <dj/error.h>

dj_int_t dj_daemon()
{

    switch (fork())
    {
    case -1:
        dj_debug("fork() failed");
        return -1;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        dj_debug("could change to root dir");
        return -2;
    }
    if (setsid() == -1)
    {
        dj_debug("\t\tsetsid() failed");
        return -3;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        dj_debug("open(\"/dev/null\") failed");
        return -4;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        dj_debug("dup2(STDIN) failed");
        return -5;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        dj_debug("dup2(STDOUT) failed");
        return -6;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            dj_debug("close() failed");
            return -7;
        }
    }
    return DJ_OK;
}
