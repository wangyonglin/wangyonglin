#if !defined(INCLUDE_MAINCONFIG_H)
#define INCLUDE_MAINCONFIG_H

#include <MainPackage.h>
#include <MainOption.h>
#include <MainLock.h>
#include <MainDaemonize.h>
#include <zlog/zlog.h>
#include <MainOption.h>
#include <MainLock.h>
#include <MemoryInject.h>

typedef struct MainConfig_s
{
    MainOption *mainOption;
    InjectObject *injectObject;
    MainLock *mainLock;
    zlog_category_t *log;
    Boolean display_errors;
    Stringex zlog_info;
    Stringex zlog_error;
} MainConfig, *pMainConfig,MainConfig_t;

void ConfigDelete();
Boolean MainConfigCreate(MainConfig **mainConfig, int argc, char *argv[]);

#endif