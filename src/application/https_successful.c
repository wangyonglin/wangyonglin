#include <application/https_successful.h>
#include <cJSON.h>
#include <wangyonglin/string.h>
#include <wangyonglin/package.h>

string_t https_failure(const char *errmsg)
{
    string_t out = string_null_command;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddFalseToObject(root, "Success");
    cJSON_AddStringToObject(root, "ErrorMessage", errmsg);
    cJSON_AddItemToObjectCS(root, "Data", cJSON_CreateObject());

    char *tmp = cJSON_PrintUnformatted(root);
    string_create(tmp, strlen(tmp));
    if (root)
        cJSON_Delete(root);
    return out;
}
string_t https_successful(cJSON *item)
{
    string_t out = string_null_command;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddTrueToObject(root, "Success");
    cJSON_AddStringToObject(root, "ErrorMessage", "");
    if (item != NULL)
        cJSON_AddItemToObjectCS(root, "Data", item);
    else
        cJSON_AddItemToObjectCS(root, "Data", cJSON_CreateObject());
    char *tmp = cJSON_PrintUnformatted(root);
    string_create(tmp, strlen(tmp));
    if (root)
        cJSON_Delete(root);
    return out;
}