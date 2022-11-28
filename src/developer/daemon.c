#include <developer/daemon.h>

int daemon_running()
{
    switch (fork())
    {
    case -1:
        fprintf(stderr, "fork() failed");
        return -1;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        fprintf(stderr, "could change to root dir");
        return -1;
    }
    if (setsid() == -1)
    {
        fprintf(stderr, "\t\tsetsid() failed");
        return -1;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open(\"/dev/null\") failed");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        fprintf(stderr, "dup2(STDIN) failed");
        return -1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "dup2(STDOUT) failed");
        return -1;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            fprintf(stderr, "close() failed");
            return -1;
        }
    }
}