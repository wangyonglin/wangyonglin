
#include <wangyonglin/config.h>
#include <wangyonglin/log.h>

ok_t daemond(config_t *config)
{

    if (config->conf->handler)
    {
        tags_t tag = conf_get_tags(config->conf, NULL, "daemon");
        if (tag == On)
        {
            switch (fork())
            {
            case -1:
                log_write(config, LOG_ERR, "fork() failed");
                config->daemon_activated = disabled;
                return failed;
            case 0:
                break;
            default:
                exit(0);
            }
            if ((chdir("/")) < 0)
            {
                log_write(config, LOG_ERR, "could change to root dir");
                config->daemon_activated = disabled;
                return failed;
            }
            if (setsid() == -1)
            {
                log_write(config, LOG_ERR, "\t\tsetsid() failed");
                config->daemon_activated = disabled;
                return failed;
            }
            umask(0);

            int fd = open("/dev/null", O_RDWR);
            if (fd == -1)
            {
                log_write(config, LOG_ERR, "open(\"/dev/null\") failed");
                config->daemon_activated = disabled;
                return failed;
            }

            if (dup2(fd, STDIN_FILENO) == -1)
            {
                log_write(config, LOG_ERR, "dup2(STDIN) failed");
                config->daemon_activated = disabled;
                return failed;
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                log_write(config, LOG_ERR, "dup2(STDOUT) failed");
                config->daemon_activated = disabled;
                return failed;
            }
            if (fd > STDERR_FILENO)
            {
                if (close(fd) == -1)
                {
                    log_write(config, LOG_ERR, "close() failed");
                    config->daemon_activated = disabled;
                    return failed;
                }
            }
            config->daemon_activated = enabled;
            return ok;
        }
    }
    return failed;
}
