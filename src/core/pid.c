#include <wangyonglin/config.h>
#include <wangyonglin/log.h>
#define PID_FILENAME "pid_filename"
ok_t pid_crt(config_t *config)
{
    if (config && config->conf->handler)
    {
        config->pid_values = getpid();
        if (config_get_string(config, NULL, PID_FILENAME, &config->pid_filename) != ok)
        {
            config->pid_filename = strdup("");
        }
        if (config->pid_filename)
        {
            int pid_fd;
            char val[16];
            int len = snprintf(val, sizeof(val), "%" PRIuMAX "\n", (uintmax_t)config->pid_values);
            if (len <= 0)
            {
                log_write(config, LOG_ERR, "Pid error (%s)", strerror(errno));
                return failed;
            }

            char str[256];
            pid_fd = open(config->pid_filename, O_CREAT | O_TRUNC | O_NOFOLLOW | O_WRONLY, 0644);
            if (pid_fd < 0)
            {
                log_write(config, LOG_ERR, "unable to set pidfile '%s': %s", config->pid_filename, strerror(errno));
                return failed;
            }
            if (lockf(pid_fd, F_TLOCK, 0) < 0)
            {
                log_write(config, LOG_ERR, "unable to lockf file: %s", strerror(errno));
                return failed;
            }
            ssize_t ret = write(pid_fd, val, (unsigned int)len);
            if (ret == -1)
            {
                log_write(config, LOG_ERR, "unable to write pidfile: %s", strerror(errno));
                close(pid_fd);
                return failed;
            }
            else if ((size_t)ret != len)
            {
                log_write(config, LOG_ERR, "unable to write pidfile: wrote"
                                           " %" PRIdMAX " of %" PRIuMAX " bytes.",
                          (intmax_t)ret, (uintmax_t)len);
                close(pid_fd);

                return failed;
            }
            close(pid_fd);
        }

        return ok;
    }

    return failed;
}

ok_t pid_del(config_t *config)
{
    if (config && config->pid_filename)
    {
        if (unlink(config->pid_filename) == 0)
        {
            return ok;
        }
        else
        {
            log_write(config, LOG_ERR, "delect unlink failed: %s", strerror(errno));
            return failed;
        }
    }
    return failed;
}
