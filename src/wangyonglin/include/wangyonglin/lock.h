#if !defined(INCLUDE_WANGYONGLIN_LOCK_H)
#define INCLUDE_WANGYONGLIN_LOCK_H

#include <wangyonglin/package.h>
#include <wangyonglin/conf.h>

typedef struct _lock_t
{
    char *lockfile;
    int lockfd;
} lock_t;

lock_t *lock_create(lock_t **lock, pool_t *pool, conf_t *conf);
ok_t lockstat(struct _lock_t *lock);
ok_t locking(struct _lock_t *lock);
ok_t unlocking(struct _lock_t *lock);
ok_t lockexit(struct _lock_t *lock);

#endif