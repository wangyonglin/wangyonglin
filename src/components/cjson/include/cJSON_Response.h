#ifndef __CJSON_RESPONSE__H__
#define __CJSON_RESPONSE__H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "cJSON.h"

typedef struct cJSON_Results
{
    cJSON *root;
    cJSON * context;
}cJSON_Response_t;


cJSON_Response_t *cJSON_Response(cJSON_Response_t **response, int errcode,const char *fmt,...);
void cJSON_Response_msg(cJSON_Response_t *obj, int err, const char *msg,...);
int cJSON_Response_out(cJSON_Response_t *obj, char **out);
void cJSON_Response_free(cJSON_Response_t * obj);
#ifdef __cplusplus
}
#endif
#endif  //!__CJSON_RESPONSE__H__