#include <ResultUtil.h>
#include <string_by_hex.h>
#include <string_by_timestamp.h>

ResultUtil *ResultUtilCreate(ResultUtil **ResUtil)
{
    object_create((void **)ResUtil, sizeof(ResultUtil));
    (*ResUtil)->Root = cJSON_CreateObject();
    return (*ResUtil);
}

boolean_by_t ResultUtilSuccessful(ResultUtil *ResUtil, cJSON *Result)
{
    boolean_by_t rc = negative;

    if (ResUtil->Root)
    {
        string_by_t id;
        string_by_hex(&id, 32);
        string_by_t timestamp;
        string_by_utc(&timestamp);
        cJSON_AddStringToObject(ResUtil->Root, "RequestId", id.valuestring);
        cJSON_AddNumberToObject(ResUtil->Root, "Code", 0);
        cJSON_AddStringToObject(ResUtil->Root, "ErrorMessage", "successful");
        cJSON_AddItemToObject(ResUtil->Root, "Result", Result);
        cJSON_AddTrueToObject(ResUtil->Root, "Success");
        cJSON_AddStringToObject(ResUtil->Root, "Timestamp", timestamp.valuestring);
        string_delete(id);
        string_delete(timestamp);
    }

    return rc;
}

boolean_by_t ResultUtilComplete(ResultUtil *ResUtil, integer_by_t Code, cJSON *Result, char *ErrorMessage)
{

    boolean_by_t rc = negative;

    if (ResUtil->Root)
    {
        string_by_t id;
        string_by_hex(&id, 32);
        string_by_t timestamp;
        string_by_utc(&timestamp);
        cJSON_AddStringToObject(ResUtil->Root, "RequestId", id.valuestring);
        cJSON_AddNumberToObject(ResUtil->Root, "Code", Code);
        if (ErrorMessage)
        {
            cJSON_AddStringToObject(ResUtil->Root, "ErrorMessage", ErrorMessage);
        }
        else
        {
            cJSON_AddStringToObject(ResUtil->Root, "ErrorMessage", "complete");
        }
        cJSON_AddItemToObject(ResUtil->Root, "Result", Result);
        cJSON_AddFalseToObject(ResUtil->Root, "Success");
        cJSON_AddStringToObject(ResUtil->Root, "Timestamp", timestamp.valuestring);
        string_delete(id);
        string_delete(timestamp);
    }

    return rc;
}

boolean_by_t ResultUtilFailure(ResultUtil *ResUtil, integer_by_t Code, char *ErrorMessage)
{

    boolean_by_t rc = negative;

    if (ResUtil->Root)
    {
        string_by_t id;
        string_by_hex(&id, 32);
        string_by_t timestamp;
        string_by_utc(&timestamp);
        cJSON_AddStringToObject(ResUtil->Root, "RequestId", id.valuestring);
        cJSON_AddNumberToObject(ResUtil->Root, "Code", Code);
        if (ErrorMessage)
        {
            cJSON_AddStringToObject(ResUtil->Root, "ErrorMessage", ErrorMessage);
        }
        else
        {
            cJSON_AddStringToObject(ResUtil->Root, "ErrorMessage", "failure");
        }

        cJSON_AddItemToObject(ResUtil->Root, "Result", cJSON_CreateObject());
        cJSON_AddFalseToObject(ResUtil->Root, "Success");
        cJSON_AddStringToObject(ResUtil->Root, "Timestamp", timestamp.valuestring);
        string_delete(id);
        string_delete(timestamp);
    }

    return rc;
}
char *ResultUtilStringify(ResultUtil *ResUtil)
{
    return cJSON_PrintUnformatted(ResUtil->Root);
}
void ResultUtilDelete(ResultUtil *ResUtil)
{
    if (ResUtil)
    {
        if (ResUtil->Root)
            cJSON_free(ResUtil->Root);
        object_delete(ResUtil);
    }
}
