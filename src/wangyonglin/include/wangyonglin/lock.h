#if !defined(INCLUDE_WANGYONGLIN_LOCK_H)
#define INCLUDE_WANGYONGLIN_LOCK_H

#include <wangyonglin/package.h>

#include <wangyonglin/log.h>
#include <string_by_this.h>

typedef struct _lock_t
{
   string_by_t pid;
   int fd;
   log_t *log;
} lock_t;

lock_t *lock_create(lock_t **lock,log_t *log);
void lock_delete(lock_t *lock);

ok_t locking(lock_t *lock);
void unlocking(lock_t *lock);
boolean_by_t locked(lock_t *lock);
ok_t lockexit(lock_t *lock);
#endif