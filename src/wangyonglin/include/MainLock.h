#if !defined(INCLUDE_MAINLOCK_H)
#define INCLUDE_MAINLOCK_H

#include <MainPackage.h>
#include <Stringex.h>
#include <zlog/zlog.h>


typedef struct MainLock_t
{
   Stringex pidFileName;
   int fd;
} MainLock;

Boolean MainLockCreate(MainLock **mainLock);
void MainLockDelete(MainLock *mainLock);
Boolean MainLockInit(MainLock *mainLock, zlog_category_t *log);
void MainLockUnlink(MainLock *mainLock);
void MainLockExit(MainLock *mainLock);
#endif