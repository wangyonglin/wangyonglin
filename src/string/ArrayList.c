#include <ArrayList.h>

int ArrayList_By_KeyString(const void *e1, const void *e2)
{

    return strcmp(((ArrayList *)e1)->keystring, ((ArrayList *)e2)->keystring);
}

void ArrayListSort(ArrayList arrayList[], size_t arrayListCount)
{
    size_t listssize = sizeof(arrayList[0]);
    qsort(arrayList, arrayListCount, listssize, ArrayList_By_KeyString);
}
