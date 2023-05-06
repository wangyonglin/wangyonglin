#include <MainFile.h>

Boolean mkdir_create(const char *dirname)
{
    Boolean err = Boolean_false;
    if (dirname)
    {
        if (0 != access(dirname, F_OK))
        {
            size_t dirname_tmplength = strlen(dirname) + 16;
            char dirname_tmpstring[dirname_tmplength];
            memset(dirname_tmpstring, 0x00, dirname_tmplength);
            sprintf(dirname_tmpstring, "mkdir -p %s", dirname);
            system(dirname_tmpstring);
        }
        err = Boolean_true;
    }
    return err;
}
