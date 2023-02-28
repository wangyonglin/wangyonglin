#include <wangyonglin/conf.h>
#include <wangyonglin/regedit.h>

conf_t *conf_create(struct _pool_t *pool, char *filename)
{
    struct _conf_t *cf;
    if (pool && filename)
    {
        object_create(pool, &cf, sizeof(struct _conf_t));
        struct _regedit_command_t commands[] = {
            {"log_errors", 0, BOOLEAN, offsetof(struct _conf_t, log_errors)},
            {"error_log", "", STRING, offsetof(struct _conf_t, error_log)},
            {"lockfile", "", STRING, offsetof(struct _conf_t, lockfile)},
            regedit_null_command};

        if (regedit(cf, pool, filename, NULL, commands) != OK)
        {
            deallocate(pool, cf);
            return NULL;
        }
        return cf;
    }
    return cf = NULL;
}