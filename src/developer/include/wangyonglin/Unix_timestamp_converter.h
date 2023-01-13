#if !defined(INCLUDE_UNIX_TIMESTAMP_CONVERTER_H)
#define INCLUDE_UNIX_TIMESTAMP_CONVERTER_H
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdbool.h>

#define log_info(format, ...) printf(format "\n", ##__VA_ARGS__)
#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))
#define _GNU_PACKED_ __attribute__((packed))

#define SECOND 1UL
#define MINU_SECONDS (60 * SECOND)
#define HOUR_SECONDS (60 * MINU_SECONDS)
#define DAY_SECONDS (24 * HOUR_SECONDS)
#define HOUR_MINUTES 60
#define TIME_UNIT 60
#define UTC_BEIJING_OFFSET_SECONDS (8 * HOUR_SECONDS)

#define UNIX_EPOCH_YEAR 1970
#define CLOSEST_FAKE_LEAP_YEAR 2102 // 2100 is nonleap year
#define UNIX_EPOCH_YEAR_WEEKDAY 4   // Thursday

#define NONLEAP_YEAR_DAYS 365
#define LEAP_YEAR_DAYS 366
#define EVERY_4YEARS_DAYS (NONLEAP_YEAR_DAYS * 3 + LEAP_YEAR_DAYS)
#define DAYS_OF_WEEK 7

typedef struct _UtcTime
{
    __uint16_t year : 12;  // max 4095
    __uint16_t month : 4;  // max 15
    __uint8_t day : 5;     // max 31
    __uint8_t weekday : 3; // max 7
    __uint8_t hour;
    __uint8_t minute;
    __uint8_t second;
} UtcTime;
bool get_is_leap_year(__uint16_t year);
UtcTime unix32_to_UTC(__uint32_t unix_time);
UtcTime unix32_to_UTC_beijing(__uint32_t unix_time);
#endif