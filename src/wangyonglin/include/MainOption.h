#if !defined(INCLUDE_MAINOPTION_H)
#define INCLUDE_MAINOPTION_H

#include <Stringex.h>

typedef struct MainOption_s
{

    Stringex confFilename;
    Boolean enabled;
    Boolean daemonize;
    Stringex logFilename;
} MainOption, *pMainOption,MainOption_t;

Boolean MainOptionCreate(MainOption **mainOption);
void MainOptionDelete(MainOption *mainOption);
Boolean MainOptionInit(MainOption *mainOption, int argc, char *argv[]);
#endif