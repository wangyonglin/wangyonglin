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

char *string_by_utc(string_by_t *utcString)
{
    char tmpstring[80];
    memset(tmpstring, 0x00, sizeof(tmpstring));
    UtcTime result;
    time_t rawtime;
    time(&rawtime);
    result = unix32_to_UTC(rawtime);
    sprintf(tmpstring, "%04d-%02d-%02dT%02d:%02d:%02dZ", result.year, result.month, result.day,
            result.hour, result.minute, result.second); // 以年月日_时分秒的形式表示当前时间
    return string_create(utcString, tmpstring, strlen(tmpstring));
}