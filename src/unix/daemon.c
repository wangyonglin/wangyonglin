
#include <unix/daemon.h>
#include <unix/log.h>

void daemonize(void)
{ // come from /redis/server.c/daemonize()
    int fd;

    if (fork() != 0)
        exit(0); /* parent exits */
    setsid();    /* create a new session */

    /* Every output goes to /dev/null. If Redis is daemonized but
     * the 'logfile' is set to 'stdout' in the configuration file
     * it will not log at all. */
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1)
    {
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO)
            close(fd);
    }
}

int daemond()
{

    switch (fork())
    {
    case -1:
        log_write(LOG_ERR, "fork() failed");
        return -1;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        log_write(LOG_ERR, "could change to root dir");

        return -2;
    }
    if (setsid() == -1)
    {
        log_write(LOG_ERR, "\t\tsetsid() failed");

        return -3;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        log_write(LOG_ERR, "open(\"/dev/null\") failed");

        return -4;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        log_write(LOG_ERR, "dup2(STDIN) failed");

        return -5;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        log_write(LOG_ERR, "dup2(STDOUT) failed");

        return -6;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            log_write(LOG_ERR, "close() failed");
            return -7;
        }
    }
    return 0;
}
