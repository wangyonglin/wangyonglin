#include <Unix_timestamp_converter.h>

bool get_is_leap_year(__uint16_t year)
{
    if (((year % 4) == 0) && ((year % 100) != 0))
    {
        return 1;
    }
    else if ((year % 400) == 0)
    {
        return 1;
    }

    return 0;
}

UtcTime unix32_to_UTC(__uint32_t unix_time)
{
    // log_info("unix_time=%u", unix_time);

    __uint8_t days_per_month[12] = {
        /*
        1   2   3   4   5   6   7   8   9   10  11  12
        */
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    struct _UtcTime utc = {0};
    __uint32_t pass_days, pass_days_cnt, pass_days_cnt_next;
    __uint8_t pass_4years_cnt;
    __uint16_t basic_4multiple_year;
    __uint16_t CurrentYear_PassDays, CurrentDay_PassMinutes;
    __uint32_t CurrentDay_PassSeconds;

    pass_days = unix_time / DAY_SECONDS;                            // passed days since UNIX_EPOCH_YEAR
    pass_4years_cnt = pass_days / EVERY_4YEARS_DAYS;                // passed how many 4 years
    basic_4multiple_year = (pass_4years_cnt * 4) + UNIX_EPOCH_YEAR; // next 4 year basic, base on UNIX_EPOCH_YEAR
    pass_days_cnt = pass_4years_cnt * EVERY_4YEARS_DAYS;            // passed day of pass_4years_cnt
    if (basic_4multiple_year >= CLOSEST_FAKE_LEAP_YEAR)
    {
        pass_days_cnt--;
    }

    // log_info("basic_4multiple_year=%d", basic_4multiple_year);
    //< get year
    for (__uint16_t i = basic_4multiple_year;; i++)
    {
        pass_days_cnt_next = get_is_leap_year(i) ? (pass_days_cnt + LEAP_YEAR_DAYS) : (pass_days_cnt + NONLEAP_YEAR_DAYS);
        if (pass_days_cnt_next > pass_days)
        {
            utc.year = i;
            break;
        }
        pass_days_cnt = pass_days_cnt_next;
    }

    //< get month
    CurrentYear_PassDays = pass_days - pass_days_cnt;
    pass_days_cnt = pass_days_cnt_next = 0;
    // log_info("CurrentYear_PassDays=%d", CurrentYear_PassDays);
    if (get_is_leap_year(utc.year))
    {
        days_per_month[1]++; // leap month of February is 29 days
    }
    for (__uint8_t i = 0; i < ARRAY_SIZE(days_per_month); i++)
    {
        pass_days_cnt_next += days_per_month[i];
        if (pass_days_cnt_next > CurrentYear_PassDays)
        {
            utc.month = i + 1;
            break;
        }
        pass_days_cnt = pass_days_cnt_next;
    }

    //< get day
    utc.day = CurrentYear_PassDays - pass_days_cnt + 1;

    //< get hour:minute:second
    CurrentDay_PassSeconds = unix_time - (pass_days * DAY_SECONDS);
    CurrentDay_PassMinutes = CurrentDay_PassSeconds / MINU_SECONDS;
    utc.hour = CurrentDay_PassMinutes / HOUR_MINUTES;
    utc.minute = CurrentDay_PassMinutes % TIME_UNIT;
    utc.second = CurrentDay_PassSeconds % TIME_UNIT;

    //< get weekday
    utc.weekday = (pass_days + UNIX_EPOCH_YEAR_WEEKDAY) % DAYS_OF_WEEK;

    // log_info("unix32_to_UTC: %d/%d/%d %02d:%02d:%02d, weekday %d",
    //           utc.year, utc.month, utc.day,
    //           utc.hour, utc.minute, utc.second,
    //           utc.weekday);

    return utc;
}

UtcTime unix32_to_UTC_beijing(__uint32_t unix_time)
{
    return unix32_to_UTC(unix_time + UTC_BEIJING_OFFSET_SECONDS);
}

const char const *weekday[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
};