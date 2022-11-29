#include <developer/program.h>
#include <developer/pidlock.h>
allocate_t *allocate = NULL;
args_t *args = NULL;
conf_t *conf = NULL;
pidlock_t *pidlock = NULL;
ok_t program_initializing(program_t **program, size_t allocate_max_size, int argc, char *argv[])
{
    if (allocate_max_size < 1024)
    {
        fprintf(stderr, "allocate_max_size < 1024\r\n");
        return ArgumentException;
    }

    if (allocate_initializing(&allocate, allocate_max_size))
    {
        if (args_initializing(&args, allocate, argc, argv) != Ok)
        {
            allocate_cleanup(allocate);
            return ErrorException;
        }

        if (conf_initializing(&conf, allocate, args->ini_filename) != Ok)
        {
            return ErrorException;
        }
        if (pidlock_initializing(&pidlock, allocate, conf) != Ok)
        {
            allocate_cleanup(allocate);
            return ErrorException;
        }

        if (pidlock_status(pidlock) != Ok)
        {
            return ErrorException;
        }
        if (object_create(allocate, (void **)program, sizeof(program_t)) != Ok)
        {
            allocate_cleanup(allocate);
            return ErrorException;
        }
        (*program)->allocate = allocate;
        (*program)->conf = conf;
        if (args->started == onstart)
        {
            printf("\tstarting\r\n");
            if (pidlock->statused == enabled)
            {
                allocate_cleanup(allocate);
                exit(EXIT_SUCCESS);
            }
            if (args->daemoned == enabled)
            {
                daemon_running();
            }
            if (pidlock_locking(pidlock) != Ok)
            {
                allocate_cleanup(allocate);
                return ErrorException;
            }

            return Ok;
        }
        else if (args->started == onstop)
        {
            printf("\tstoping\r\n");
            pidlock_exit(pidlock);
            allocate_cleanup(allocate);
            exit(EXIT_SUCCESS);
        }
    }

    printf("\tkernelInitializing\r\n");
    return ErrorException;
}

void program_cleanup(program_t *program)
{
    if (program)
    {
        allocate_cleanup(allocate);
    }
}
