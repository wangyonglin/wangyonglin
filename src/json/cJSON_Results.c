#include <json/cJSON_Results.h>
#include <wangyonglin/linux.h>

cJSON_Results_t *cJSON_Results_new(int errcode)
{
    cJSON_Results_t *obj = (cJSON_Results_t *)malloc(sizeof(cJSON_Results_t));
    if (obj)
    {
        obj->root = cJSON_CreateObject();
        obj->context = cJSON_CreateObject();
        if (obj->root && obj->context)
        {

            if (errcode == 200)
                cJSON_AddTrueToObject(obj->root, "success");

            else
                cJSON_AddFalseToObject(obj->root, "success");
            cJSON_AddNumberToObject(obj->root, "errcode", errcode);
            cJSON_AddItemToObject(obj->root, "result", obj->context);

            switch (errcode)
            {
            case 200:
                cJSON_AddStringToObject(obj->root, "reason", "200 OK");
                break;
            case 400:
                cJSON_AddStringToObject(obj->root, "reason", "400 Bad Request");
                break;
            case 404:
                cJSON_AddStringToObject(obj->root, "reason", "404 Not Found");
                break;
            default:
                cJSON_AddStringToObject(obj->root, "reason", "Unknown Error");
                break;
            }
            return obj;
        }
    }

    return NULL;
}
int cJSON_Results_Print(cJSON_Results_t *obj, char **out)
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
void cJSON_Results_free(cJSON_Results_t *obj)
{
    if (obj)
    {
        if (obj->root)
            cJSON_Delete(obj->root);
        free(obj);
    }
}