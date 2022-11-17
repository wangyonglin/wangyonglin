#if !defined(INCLUDE_SYSTEMLOG_H)
#define INCLUDE_SYSTEMLOG_H

#include <SystemTypes.h>
#include <AllocateUtils.h>
#include <ConfUtils.h>
typedef enum
{
    LOG_ERR = -1,
    LOG_INFO,
    LOG_NONE,
    LOG_DEBUG,
} log_priority_t;

typedef struct
{
    char *log_file;
    bool log_debug;
    AllocateUtils_t *AllocateUtils;
} SystemLog_t;

ok_t SystemLog_initializing(SystemLog_t **SystemLog, AllocateUtils_t *AllocateUtils, ConfUtils_t *ConfUtils);

int SystemLog_error(SystemLog_t *SystemLog,const char *fmt, ...) __attribute__((format(printf, 2, 3)));
int SystemLog_info(SystemLog_t *SystemLog,const char *fmt, ...) __attribute__((format(printf, 2, 3)));

#endif //!__LOG__H__