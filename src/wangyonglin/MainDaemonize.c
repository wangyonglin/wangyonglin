#include <MainDaemonize.h>

Boolean daemonize()
{
    Boolean err = Boolean_false;
    switch (fork())
    {
    case -1:
        fprintf(stderr, "fork() failed");
        return err;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        fprintf(stderr, "could change to root dir");
        return err;
    }
    if (setsid() == -1)
    {
        fprintf(stderr, "\t\tsetsid() failed");
        return err;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open(\"/dev/null\") failed");
        return err;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        fprintf(stderr, "dup2(STDIN) failed");
        return err;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "dup2(STDOUT) failed");
        return err;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            fprintf(stderr, "close() failed");
            return err;
        }
    }
    return Boolean_true;
}