
#include <wangyonglin/config.h>
#include <wangyonglin/log.h>

activated daemond(config_t *config)
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
                return config->daemon_activated = disabled;
            case 0:
                break;
            default:
                exit(0);
            }
            if ((chdir("/")) < 0)
            {
                log_write(config, LOG_ERR, "could change to root dir");
                return config->daemon_activated = disabled;
            }
            if (setsid() == -1)
            {
                log_write(config, LOG_ERR, "\t\tsetsid() failed");
                return config->daemon_activated = disabled;
            }
            umask(0);

            int fd = open("/dev/null", O_RDWR);
            if (fd == -1)
            {
                log_write(config, LOG_ERR, "open(\"/dev/null\") failed");
                return config->daemon_activated = disabled;
            }

            if (dup2(fd, STDIN_FILENO) == -1)
            {
                log_write(config, LOG_ERR, "dup2(STDIN) failed");
                return config->daemon_activated = disabled;
            }

            if (dup2(fd, STDOUT_FILENO) == -1)
            {
                log_write(config, LOG_ERR, "dup2(STDOUT) failed");
                return config->daemon_activated = disabled;
            }
            if (fd > STDERR_FILENO)
            {
                if (close(fd) == -1)
                {
                    log_write(config, LOG_ERR, "close() failed");
                    return config->daemon_activated = disabled;
                }
            }
            return config->daemon_activated = enabled;
        }
    }
    return  disabled;
}
