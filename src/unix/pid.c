#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <getopt.h>
#include <signal.h>
#include <unix/string.h>
#include <unix/pid.h>
#include <unix/log.h>
#include <unix/unix.h>
pid_mk_t *pid_crt(pid_mk_t **pid, log_t *log, const char *filename)
{
    if ((*pid) = (pid_mk_t *)global_hooks.allocate(sizeof(pid_mk_t)))
    {
        if (string_crt(&((*pid)->filename), filename, string_len(filename)))
        {
            int pid_fd;
            char val[16];
            int len = snprintf(val, sizeof(val), "%" PRIuMAX "\n", (uintmax_t)getpid());
            if (len <= 0)
            {
                log_write(log, LOG_ERR, "pid error (%s)", strerror(errno));
                global_hooks.deallocate((*pid));
                return NULL;
            }
            char str[256];
            pid_fd = open(filename, O_CREAT | O_TRUNC | O_NOFOLLOW | O_WRONLY, 0644);
            if (pid_fd < 0)
            {
                log_write(log, LOG_ERR, "unable to set pidfile '%s': %s", (*pid)->filename, strerror(errno));
                global_hooks.deallocate((*pid));
                return NULL;
            }
            if (lockf(pid_fd, F_TLOCK, 0) < 0)
            {
                log_write(log, LOG_ERR, "unable to lockf file: %s", strerror(errno));
                return NULL;
            }
            ssize_t ret = write(pid_fd, val, (unsigned int)len);
            if (ret == -1)
            {
                log_write(log, LOG_ERR, "unable to write pidfile: %s", strerror(errno));
                close(pid_fd);
                return NULL;
            }
            else if ((size_t)ret != len)
            {
                log_write(log, LOG_ERR, "unable to write pidfile: wrote"
                                        " %" PRIdMAX " of %" PRIuMAX " bytes.",
                          (intmax_t)ret, (uintmax_t)len);
                close(pid_fd);
                return NULL;
            }
            return (*pid);
        }
    }

    return NULL;
}

void pid_del(pid_mk_t *pid, log_t *log)
{
    if (pid && pid->filename)
        if (unlink(pid->filename) != 0)
        {
            log_write(log, LOG_ERR, "delect unlink failed:[%s] %s ", pid->filename, strerror(errno));
        }
}
