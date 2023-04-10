#include <https_result.h>

void https_result_delete(https_result *result)
{
    if (result)
    {
        if (result->jsonformat)
            free(result->jsonformat);
        result->jsonsize = 0;
        result->code = 0;
        free(result);
    }
}
https_result *https_result_create(https_result **result)
{
    (*result) = (https_result *)malloc(sizeof(https_result));
    if ((*result))
    {

        if (((*result)->jsonformat = (char *)malloc(sizeof(char))))
        {
            (*result)->jsonsize = 0;
            (*result)->code = 0;
            return (*result);
        }
        free((*result));
        return NULL;
    }
    return NULL;
}

size_t WriteMemoryCallback(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    https_result *mem = (https_result *)data;

    mem->jsonformat = (char *)realloc(mem->jsonformat, mem->jsonsize + realsize + 1);
    if (mem->jsonformat)
    {
        memcpy(&(mem->jsonformat[mem->jsonsize]), ptr, realsize);
        mem->jsonsize += realsize;
        mem->jsonformat[mem->jsonsize] = 0;
    }
    return realsize;
}