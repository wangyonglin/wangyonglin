#include <wangyonglin/list.h>

int _list_by_keystring(const void *e1, const void *e2)
{

    return strcmp(((struct _list_t *)e1)->keystring, ((struct _list_t *)e2)->keystring);
}

void list_sort(struct _list_t lists[], size_t listscount)
{
    size_t listssize = sizeof(lists[0]);
    qsort(lists, listscount, listssize, _list_by_keystring);
}
