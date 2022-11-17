#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <SystemDaemon.h>
#include <SystemError.h>
ConfUtils_command_t SystemDaemon_commands[] = {
    {"deamoned", boolean, offsetof(SystemDaemon_t, deamoned)}};

ok_t SystemDaemon_initializing(SystemDaemon_t **SystemDaemon, AllocateUtils_t *AllocateUtils, ConfUtils_t *ConfUtils)
{
    if (!AllocateUtils && !ConfUtils)
    {
        return ArgumentException;
    }

    if (((*SystemDaemon) = AllocateUtils_pool(AllocateUtils, sizeof(SystemLog_t))) == NULL)
    {
        return NullPointerException;
    }
    int SystemDaemon_commands_size = sizeof(SystemDaemon_commands) / sizeof(SystemDaemon_commands[0]);
    ConfUtils_final(ConfUtils, SystemDaemon, sizeof(SystemDaemon_t), NULL, SystemDaemon_commands, SystemDaemon_commands_size);

    return OK;
    (*SystemDaemon)->AllocateUtils = AllocateUtils;
    return ErrorException;
}
ok_t SystemDaemon_start(SystemDaemon_t *SystemDaemon)
{
    if (!SystemDaemon)
    {
        return ArgumentException;
    }
    if (SystemDaemon->deamoned == false)
        return OK;
    switch (fork())
    {
    case -1:
        SystemError_exitMessage(SystemDaemon->AllocateUtils, "fork() failed");
        return -1;
    case 0:
        break;
    default:
        exit(0);
    }
    if ((chdir("/")) < 0)
    {
        SystemError_exitMessage(SystemDaemon->AllocateUtils, "could change to root dir");
        return -1;
    }
    if (setsid() == -1)
    {
        SystemError_exitMessage(SystemDaemon->AllocateUtils, "setsid() failed");
        return -1;
    }
    umask(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd == -1)
    {
        SystemError_exitMessage(SystemDaemon->AllocateUtils, "open(\"/dev/null\") failed");
        return -1;
    }

    if (dup2(fd, STDIN_FILENO) == -1)
    {
        SystemError_exitMessage(SystemDaemon->AllocateUtils, "dup2(STDIN) failed");
        return -1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        SystemError_exitMessage(SystemDaemon->AllocateUtils, "dup2(STDOUT) failed");
        return -1;
    }
    if (fd > STDERR_FILENO)
    {
        if (close(fd) == -1)
        {
            SystemError_exitMessage(SystemDaemon->AllocateUtils, "close() failed");
            return -1;
        }
    }
    return 0;
}
