#if !defined(INCLUDE_RESULTUTIL_H)
#define INCLUDE_RESULTUTIL_H
#include <cJSON.h>
#include <string_by_id.h>
typedef struct _ResultUtil_t
{
    cJSON *Root;
} ResultUtil;

ResultUtil *ResultUtilCreate(ResultUtil **ResUtil);

boolean_by_t ResultUtilSuccessful(ResultUtil *ResUtil,cJSON * Result);
boolean_by_t ResultUtilComplete(ResultUtil *ResUtil, integer_by_t Code, cJSON *Result, char *ErrorMessage);
boolean_by_t ResultUtilFailure(ResultUtil *ResUtil, integer_by_t Code, char *ErrorMessage);
void ResultUtilDelete(ResultUtil *ResUtil);
char *ResultUtilStringify(ResultUtil *ResUtil);
#endif