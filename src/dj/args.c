#include <dj/args.h>
#include <dj/string.h>
#include <dj/error.h>
dj_int_t dj_args_init(dj_args_t **args,int argc, char *argv[])
{
  
    int i;
    if (((*args) = (dj_args_t *)global_hooks.allocate(sizeof(dj_args_t))))
    {
        (*args)->deamon = DJ_BOOL_FALSE;
        (*args)->started = DJ_STARTED_START;
        for (i = 1; i < argc; i++)
        {
            if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--args-file"))
            {
                if (i < argc - 1)
                {
                    dj_string_crt(&((*args)->conf), argv[i + 1], strlen(argv[i + 1]));
                }
                else
                {
                    dj_debug("Error: -c argument given, but no args file specified.");
                    return DJ_ARGS_ERROR;
                }

                i++;
            }
            else if (!strcmp(argv[i], "-s") || !strcmp(argv[i], "--start"))
            {
                if (i < argc - 1)
                {
                    if (!strcmp(argv[i + 1], "start"))
                    {
                        (*args)->started = DJ_STARTED_START;
                    }
                    else if (!strcmp(argv[i + 1], "stop"))
                    {
                        (*args)->started = DJ_STARTED_STOP;
                    }
                }
                else
                {
                    dj_debug( "Error: -s argument given, but no port specified.");
                    return DJ_ARGS_ERROR;
                }
                i++;
            }
            else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--daemon"))
            {
                (*args)->deamon = DJ_BOOL_TRUE;
            }
            else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
            {

                return DJ_ARGS_NULL;
            }
        }
        if ((*args)->conf == NULL)
        {
            dj_string_crt(&((*args)->conf), "/home/wangyonglin/github/wangyonglin/conf/tiger.conf", strlen("/home/wangyonglin/github/wangyonglin/conf/tiger.conf"));
        }
        return DJ_ARGS_OK;
    }
    return DJ_ARGS_NULL;
}

void dj_args_clean(dj_args_t *args)
{
    if (args)
    {
        string_del(args->conf);
        global_hooks.deallocate(args);
    }
}