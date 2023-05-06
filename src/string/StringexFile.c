#include <StringexFile.h>

Boolean StringexFileDataCallback(Stringex *callStringex, char *filename)
{
    Boolean err = Boolean_false;
    if (filename)
    {

        FILE *fpr = fopen(filename, "r");
        if (fpr)
        {
            fseek(fpr, 0L, SEEK_END); // 将文件指针移动到文件末尾
            long size = ftell(fpr);   // 获取文件大小
            fseek(fpr, 0L, SEEK_SET);
            char tmpString[size];
            memset(tmpString, 0x00, size);
            size_t rsize = fread(tmpString, sizeof(char), size, fpr);
            fclose(fpr);
            if (StringexCreate(callStringex, tmpString, rsize))
            {
                return Boolean_true;
            }
        }
    }

    return err;
}