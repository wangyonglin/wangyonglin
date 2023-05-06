
#if !defined(INCLUDE_ARRAYLIST_H)
#define INCLUDE_ARRAYLIST_H

#include <Stringex.h>
#define ARRAYLIST_NULL_COMMAND \
    {                          \
        NULL, NULL, 0, -1      \
    }
#define ARRAYLIST_STRING_COMMAND(keystring, valstring) \
    {                                                  \
        keystring, valstring, 0, OBJECT_TYPE_STRING                \
    }
#define ARRAYLIST_INTEGER_COMMAND(keystring, valinteger) \
    {                                                    \
        keystring, NULL, valinteger, OBJECT_TYPE_INTEGER             \
    }
#define ArrayListCount(lists) sizeof(lists) / sizeof(lists[0])

typedef struct ArrayList_s
{
    char *keystring;
    char *valstring;
    size_t valinteger;
    ObjectType valtype;
} ArrayList, ArrayList_t;

void ArrayListSort(ArrayList arrayList[], size_t arrayListCount);

#endif
