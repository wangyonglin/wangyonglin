#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>

void *allocate(void **obj, size_t size)
{

    if ((*obj) = (obj*)malloc(sizeof(*obj) * size))
    {
        memset(*obj,0x00,sizeof(*obj) * size);
        return *obj;
    }
    return NULL;
}
void deallocate(void * obj){
    if(obj)(free(obj));
}
typedef struct 
{
    char * ddd;
}test_t;

int main(int args,char **argv){

    test_t * test=NULL;
    if(allocate((**test_t)(&test),1)){


        deallocate(test);
    }

    return 0;

}