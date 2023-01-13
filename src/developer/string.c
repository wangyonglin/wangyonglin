#include <wangyonglin/string.h>

char *strcrt(char **obj, char *dat, size_t datsz)
{

    if ((*obj) = (char *)global_hooks.allocate(sizeof(char) * datsz + 1))
    {
        memset((*obj), 0x00, sizeof(char) * datsz + 1);
        memcpy((*obj), dat, datsz);
        return (*obj);
    }
    return NULL;
}

char *strnull(char **obj, size_t objmaxsz)
{
    if ((*obj) = (char *)global_hooks.allocate(sizeof(char) * objmaxsz))
    {
        memset((*obj), '\0', sizeof(char) * objmaxsz);
        return (*obj);
    }
    return NULL;
}
void strdel(char *obj)
{
    if (obj)
        global_hooks.deallocate(obj);
}

void *object_create(void **pointer, size_t pointer_size)
{
    if ((*pointer) = global_hooks.allocate(pointer_size))
    {
        memset((*pointer), 0x00, pointer_size);
        return (*pointer);
    }
    return NULL;
}
void object_delete(void *pointer)
{
    if (pointer)
        free(pointer);
}

void boolean_create(unsigned char **pointer, boolean value)
{
    *pointer = (unsigned char *)value;
}
void integer_create(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}

// strdat *strdat_crt(strdat **pointer, char *data, size_t data_size)
// {
//     if ((*pointer) = (strdat *)global_hooks.allocate(sizeof(strdat) * data_size))
//     {
//         memset((*pointer), 0x00, sizeof(char) * data_size);
//         (*pointer)->len = data_size;
//         if ((*pointer)->dat = global_hooks.allocate(sizeof(char) * data_size + 1))
//         {
//             memcpy((*pointer)->dat, data, (*pointer)->len);
//             return (*pointer);
//         }
//         else
//         {
//             global_hooks.deallocate(*pointer);
//         }
//     }
//     return NULL;
// }
// void strdat_del(strdat *pointer)
// {
//     if (pointer)
//     {
//         if (pointer->dat)
//             global_hooks.deallocate(pointer->dat);
//         pointer->len = 0;
//         global_hooks.deallocate(pointer);
//     }
// }
void string_rows(const char *title, char *data)
{
    printf("\t%s:[%d]\n \t\t%s\r\n", title, strlen(data), data);
}

void *memcrt(void **obj, size_t objsz, void *dat, size_t datsz)
{
    if ((*obj) = (void *)global_hooks.allocate(objsz))
    {
        memset((*obj), 0x00, objsz);
        memcpy((*obj), dat, datsz);
        return (*obj);
    }
    return NULL;
}
void *memnull(void **obj, size_t objsz)
{
    if ((*obj) = (void *)global_hooks.allocate(objsz))
    {
        memset((*obj), 0x00, objsz);
        return (*obj);
    }
    return NULL;
}
void memdel(void * obj){
    if(obj)global_hooks.deallocate(obj);
}