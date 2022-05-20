
#include <wangyonglin/config.h>
#include <wangyonglin/log.h>

activated daemond(config_t *config)
{

    switch (fork())
    {
    case -1:
        fprintf(stderr, "fork() failed");
        return failed;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        fprintf(stderr, "could change to root dir");
        return failed;
    }
    if (setsid() == -1)
    {
        fprintf(stderr, "\t\tsetsid() failed");
        return failed;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        fprintf(stderr, "open(\"/dev/null\") failed");
        return failed;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        fprintf(stderr, "dup2(STDIN) failed");
        return failed;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        fprintf(stderr, "dup2(STDOUT) failed");
        return failed;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            fprintf(stderr, "close() failed");
            return failed;
        }
    }
    return ok;
}
