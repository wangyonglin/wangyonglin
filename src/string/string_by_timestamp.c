#include <string_by_timestamp.h>

char *string_by_timestamp(string_by_t *timestamp)
{
    char toString[16] = {0};
    struct timeval tv;
    gettimeofday(&tv, NULL);
    // long result = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    // printf("second:%ld\n", tv.tv_sec); // 秒
    // printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000); // 毫秒
    // printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec); // 微秒
    sprintf(toString, "%ld", tv.tv_sec);
    return string_create(timestamp, toString, strlen(toString));
}
