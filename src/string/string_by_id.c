#include <string_by_id.h>
#include <SnowFlake.h>
#include <stdio.h>
#include <string_by_this.h>
char *string_by_id(string_by_t *id)
{

    char toString[32];
    bzero(toString, sizeof(toString));
    int64_t result = SnowFlake_IdGenerator();
    sprintf(toString, "%ld", result);

    if (string_create(id, toString, strlen(toString)))
    {
        return id->valuestring;
    }
    return NULL;
}