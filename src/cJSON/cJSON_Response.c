#include <cJSON_Response.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
cJSON_Response_t *cJSON_Response(cJSON_Response_t **result, int errcode, const char *fmt, ...)
{
    char reason[1024] = {0};
    va_list va;
    int ret;
    va_start(va, fmt);
    ret = vsprintf(reason, fmt, va);
    va_end(va);

    (*result) = (cJSON_Response_t *)malloc(sizeof(cJSON_Response_t));
    if ((*result))
    {
        (*result)->root = cJSON_CreateObject();
        (*result)->context = cJSON_CreateObject();
        if ((*result)->root && (*result)->context)
        {

            if (errcode == 200)
                cJSON_AddTrueToObject((*result)->root, "success");

            else
                cJSON_AddFalseToObject((*result)->root, "success");
            cJSON_AddNumberToObject((*result)->root, "errcode", errcode);
            cJSON_AddItemToObject((*result)->root, "result", (*result)->context);
            if (reason)
            {
                cJSON_AddStringToObject((*result)->root, "reason", reason);
            }
            else
            {
                cJSON_AddStringToObject((*result)->root, "reason", "Unknown Error");
            }

            return (*result);
        }
    }

    return NULL;
}
void cJSON_Response_msg(cJSON_Response_t *obj, int err, const char *msg, ...)
{

    if (obj && obj->context)
    {
        char msgbuf[1024] = {0};
        va_list va;
        int ret;
        va_start(va, msg);
        ret = vsprintf(msgbuf, msg, va);
        va_end(va);
        cJSON_AddNumberToObject(obj->context, "error", err);
        cJSON_AddStringToObject(obj->context, "message", msgbuf);
    }
}
int cJSON_Response_out(cJSON_Response_t *obj, char **out)
{

    if (obj->root)
    {
        char *ret = cJSON_Print(obj->root);
        int len = strlen(ret);
        *out = (char *)malloc(sizeof(char) * len + 1);
        memset(*out, 0x00, len + 1);
        strncpy(*out, ret, len);
        free(ret);
        return len;
    }

    return 0;
}
void cJSON_Response_free(cJSON_Response_t *obj)
{
    if (obj)
    {
        if (obj->root)
            cJSON_Delete(obj->root);
        free(obj);
    }
}