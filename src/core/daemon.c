
#include <wangyonglin/config.h>
#include <wangyonglin/log.h>

ok_t daemond(config_t *config)
{
    if (config)
    {
        switch (fork())
        {
        case -1:
            log_write(config, LOG_ERR, "fork() failed");
            return failed;
        case 0:
            break;
        default:
            exit(0);
        }
        if ((chdir("/")) < 0)
        {
            log_write(config, LOG_ERR, "could change to root dir");

            return failed;
        }
        if (setsid() == -1)
        {
            log_write(config, LOG_ERR, "\t\tsetsid() failed");

            return failed;
        }
        umask(0);

        int fd = open("/dev/null", O_RDWR);
        if (fd == -1)
        {
            log_write(config, LOG_ERR, "open(\"/dev/null\") failed");

            return failed;
        }

        if (dup2(fd, STDIN_FILENO) == -1)
        {
            log_write(config, LOG_ERR, "dup2(STDIN) failed");

            return failed;
        }

        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            log_write(config, LOG_ERR, "dup2(STDOUT) failed");

            return failed;
        }
        if (fd > STDERR_FILENO)
        {
            if (close(fd) == -1)
            {
                log_write(config, LOG_ERR, "close() failed");
                return failed;
            }
        }
        return ok;
    }
    return null;
}
