#ifndef __CJSON_RESULTS__H__
#define __CJSON_RESULTS__H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "cJSON.h"

typedef struct cJSON_Results
{
    cJSON *root;
    cJSON * context;
}cJSON_Results_t;

cJSON_Results_t *cJSON_Results_new(int errcode);
int cJSON_Results_Print(cJSON_Results_t *obj, char **out);
void cJSON_Results_free(cJSON_Results_t * obj);
#ifdef __cplusplus
}
#endif
#endif  //!__CJSON_RESULTS__H__