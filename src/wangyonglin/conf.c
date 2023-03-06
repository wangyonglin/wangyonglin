#include <wangyonglin/conf.h>
#include <wangyonglin/object.h>

conf_t *conf_create(conf_t **conf, pool_t *pool, string_t confname)
{

    if (pool)
    {
        if (pool_object_create(pool, (void **)conf, sizeof(conf_t)))
        {
            object_command_t objects[] = {
                object_string_command("error_log", offsetof(conf_t, error_log)),
                object_string_command("lockfile", offsetof(conf_t, lockfile)),
                object_boolean_command("log_errors", offsetof(conf_t, log_errors)),
                object_null_command};
            object_mirror(objects, (*conf), confname, NULL);
            return (*conf);
        }
    }
    return (*conf) = NULL;
}