#include <application/https_successful.h>
#include <cJSON.h>
#include <wangyonglin/string.h>
#include <wangyonglin/package.h>
char *https_failure(char **ostr, const char *errmsg)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddFalseToObject(root, "Success");
    cJSON_AddStringToObject(root, "ErrorMessage", errmsg);
    cJSON_AddItemToObjectCS(root, "Data", cJSON_CreateObject());

    char *out = cJSON_PrintUnformatted(root);
    size_t outsize = strlen(out);
    strcrt(ostr, out, outsize);
    if (root)
        cJSON_Delete(root);
    return ostr;
}
char *https_successful(char **pstr, cJSON *item)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "Success");
    cJSON_AddStringToObject(root, "ErrorMessage", "");
    if (item != NULL)
        cJSON_AddItemToObjectCS(root, "Data", item);
    else
        cJSON_AddItemToObjectCS(root, "Data", cJSON_CreateObject());
    char *out = cJSON_PrintUnformatted(root);
    size_t outsize = strlen(out);
    strcrt(pstr, out, outsize);
    if (root)
        cJSON_Delete(root);
    return pstr;
}