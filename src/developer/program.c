#include <developer/program.h>
#include <developer/pidlock.h>
allocate_t *allocate = NULL;
args_t *args = NULL;
conf_t *conf = NULL;
pidlock_t *pidlock = NULL;
logger_t *logger = NULL;

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
            fprintf(stderr, "args_initializing failed\r\n");
            allocate_cleanup(allocate);
            return ErrorException;
        }

        if (conf_initializing(&conf, allocate, args->ini_filename) != Ok)
        {
            fprintf(stderr, "conf_initializing failed\r\n");
            return ErrorException;
        }
        if (pidlock_initializing(&pidlock, allocate, conf) != Ok)
        {
            fprintf(stderr, "pidlock_initializing failed\r\n");
            allocate_cleanup(allocate);
            return ErrorException;
        }

        if (pidlock_status(pidlock) != Ok)
        {
            return ErrorException;
        }
        if (logger_initializing(&logger, allocate, conf, args->daemoned) != Ok)
        {
            fprintf(stderr, "logger_initializing failed\r\n");
            allocate_cleanup(allocate);
            return ErrorException;
        }
        if (object_create(allocate, (void **)program, sizeof(program_t)) != Ok)
        {
            fprintf(stderr, "object_create->program failed\r\n");
            allocate_cleanup(allocate);
            return ErrorException;
        }
        (*program)->allocate = allocate;
        (*program)->conf = conf;
        (*program)->logger = logger;
        if (args->started == onstart)
        {

            fprintf(stdout, "\tstarting succeed\r\n");
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
            fprintf(stdout, "\tstoping succeed\r\n");
            pidlock_exit(pidlock);
            allocate_cleanup(allocate);
            exit(EXIT_SUCCESS);
        }
        else if (args->started == onstatus)
        {

            if (pidlock->statused == enabled)
            {
                fprintf(stdout, "\tRunning\r\n");
            }
            else
            {
                fprintf(stdout, "\tNot Running\r\n");
            }
            allocate_cleanup(allocate);
            exit(EXIT_SUCCESS);
        }
    }
    return ErrorException;
}

void program_cleanup(program_t *program)
{
    if (program)
    {
        allocate_cleanup(allocate);
    }
}
