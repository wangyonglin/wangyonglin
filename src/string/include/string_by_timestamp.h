#if !defined(UTILS_INCLUDE_STRING_BY_TIMESTAMP_H)
#define UTILS_INCLUDE_STRING_BY_TIMESTAMP_H
#include <stdio.h>  
#include <sys/time.h>    
#include <string_by_this.h>
#include <Unix_timestamp_converter.h>
/**
 * @brief 
 * 
 * @param timestamp 
 * @return char* 
 */
char* string_by_timestamp(string_by_t *timestamp);

/**
 * @brief 格式为YYYY-MM-ddTHH:mm:ssZ
 * 
 * @param utcString 
 * @return char* 
 */
char *string_by_utc(string_by_t *utcString);
#endif