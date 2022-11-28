#include <developer/kernel.h>


allocate_t *allocate = NULL;
args_t *args = NULL;
conf_t *conf = NULL;


ok_t initializing(kernel_t **kernel, size_t allocate_max_size, int argc, char *argv[])
{
    if (allocate_max_size < 1024)
    {
        fprintf(stderr, "allocate_max_size < 1024\r\n");
        return ArgumentException;
    }

    if (allocate_initializing(&allocate, allocate_max_size))
    {

        if (object_crt(allocate, (void**)kernel, sizeof(kernel_t)) != Ok)
        {
            return NullPointerException;
        }
        if (args_initializing(&args, allocate, argc, argv) != Ok)
        {
            return ErrorException;
        }
        printf("\tstarted{%d}\r\n", args->started);
        printf("\tdaemoned{%d}\r\n", args->daemoned);
        printf("\tini_filename{%s}\r\n", args->ini_filename);
        if (conf_initializing(&conf, allocate, args->ini_filename) != Ok)
        {
            return ErrorException;
        }
        (*kernel)->allocate=allocate;
        (*kernel)->conf=conf;
        (*kernel)->args=args;
        return Ok;
    }

    printf("\tkernelInitializing\r\n");
    return ErrorException;
}
void cleanup(kernel_t *kernel)
{
    if (kernel)
    {
        allocate_cleanup(allocate);
    }
}
