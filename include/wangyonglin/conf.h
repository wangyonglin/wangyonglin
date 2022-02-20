/*
  MIT License
  Copyright (c) CK Tan
  https://github.com/cktan/tomlc99
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#ifndef __CONF__H__
#define __CONF__H__

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
#define CONF_EXTERN extern "C"
#else
#define CONF_EXTERN extern
#endif

typedef struct conf_timestamp_t conf_timestamp_t;
typedef struct conf_table_t conf_table_t;
typedef struct conf_array_t conf_array_t;
typedef struct conf_datum_t conf_datum_t;

/* Parse a file. Return a table on success, or 0 otherwise.
 * Caller must conf_free(the-return-value) after use.
 */
CONF_EXTERN conf_table_t *conf_parse_file(FILE *fp, char *errbuf, int errbufsz);

/* Parse a string containing the full config.
 * Return a table on success, or 0 otherwise.
 * Caller must conf_free(the-return-value) after use.
 */
CONF_EXTERN conf_table_t *conf_parse(char *conf, /* NUL terminated, please. */
                                     char *errbuf, int errbufsz);

/* Free the table returned by conf_parse() or conf_parse_file(). Once
 * this function is called, any handles accessed through this tab
 * directly or indirectly are no longer valid.
 */
CONF_EXTERN void conf_free(conf_table_t *tab);

/* Timestamp types. The year, month, day, hour, minute, second, z
 * fields may be NULL if they are not relevant. e.g. In a DATE
 * type, the hour, minute, second and z fields will be NULLs.
 */
struct conf_timestamp_t {
  struct { /* internal. do not use. */
    int year, month, day;
    int hour, minute, second, millisec;
    char z[10];
  } __buffer;
  int *year, *month, *day;
  int *hour, *minute, *second, *millisec;
  char *z;
};

/*-----------------------------------------------------------------
 *  Enhanced access methods
 */
struct conf_datum_t {
  int ok;
  union {
    conf_timestamp_t *ts; /* ts must be freed after use */
    char *s;              /* string value. s must be freed after use */
    int b;                /* bool value */
    int64_t i;            /* int value */
    double d;             /* double value */
  } u;
};

/* on arrays: */
/* ... retrieve size of array. */
CONF_EXTERN int conf_array_nelem(const conf_array_t *arr);
/* ... retrieve values using index. */
CONF_EXTERN conf_datum_t conf_string_at(const conf_array_t *arr, int idx);
CONF_EXTERN conf_datum_t conf_bool_at(const conf_array_t *arr, int idx);
CONF_EXTERN conf_datum_t conf_int_at(const conf_array_t *arr, int idx);
CONF_EXTERN conf_datum_t conf_double_at(const conf_array_t *arr, int idx);
CONF_EXTERN conf_datum_t conf_timestamp_at(const conf_array_t *arr, int idx);
/* ... retrieve array or table using index. */
CONF_EXTERN conf_array_t *conf_array_at(const conf_array_t *arr, int idx);
CONF_EXTERN conf_table_t *conf_table_at(const conf_array_t *arr, int idx);

/* on tables: */
/* ... retrieve the key in table at keyidx. Return 0 if out of range. */
CONF_EXTERN const char *conf_key_in(const conf_table_t *tab, int keyidx);
/* ... returns 1 if key exists in tab, 0 otherwise */
CONF_EXTERN int conf_key_exists(const conf_table_t *tab, const char *key);
/* ... retrieve values using key. */
CONF_EXTERN conf_datum_t conf_string_in(const conf_table_t *arr,
                                        const char *key);
CONF_EXTERN conf_datum_t conf_bool_in(const conf_table_t *arr, const char *key);
CONF_EXTERN conf_datum_t conf_int_in(const conf_table_t *arr, const char *key);
CONF_EXTERN conf_datum_t conf_double_in(const conf_table_t *arr,
                                        const char *key);
CONF_EXTERN conf_datum_t conf_timestamp_in(const conf_table_t *arr,
                                           const char *key);
/* .. retrieve array or table using key. */
CONF_EXTERN conf_array_t *conf_array_in(const conf_table_t *tab,
                                        const char *key);
CONF_EXTERN conf_table_t *conf_table_in(const conf_table_t *tab,
                                        const char *key);

/*-----------------------------------------------------------------
 * lesser used
 */
/* Return the array kind: 't'able, 'a'rray, 'v'alue, 'm'ixed */
CONF_EXTERN char conf_array_kind(const conf_array_t *arr);

/* For array kind 'v'alue, return the type of values
   i:int, d:double, b:bool, s:string, t:time, D:date, T:timestamp, 'm'ixed
   0 if unknown
*/
CONF_EXTERN char conf_array_type(const conf_array_t *arr);

/* Return the key of an array */
CONF_EXTERN const char *conf_array_key(const conf_array_t *arr);

/* Return the number of key-values in a table */
CONF_EXTERN int conf_table_nkval(const conf_table_t *tab);

/* Return the number of arrays in a table */
CONF_EXTERN int conf_table_narr(const conf_table_t *tab);

/* Return the number of sub-tables in a table */
CONF_EXTERN int conf_table_ntab(const conf_table_t *tab);

/* Return the key of a table*/
CONF_EXTERN const char *conf_table_key(const conf_table_t *tab);

/*--------------------------------------------------------------
 * misc
 */
CONF_EXTERN int conf_utf8_to_ucs(const char *orig, int len, int64_t *ret);
CONF_EXTERN int conf_ucs_to_utf8(int64_t code, char buf[6]);
CONF_EXTERN void conf_set_memutil(void *(*xxmalloc)(size_t),
                                  void (*xxfree)(void *));

/*--------------------------------------------------------------
 *  deprecated
 */
/* A raw value, must be processed by conf_rto* before using. */
typedef const char *conf_raw_t;
CONF_EXTERN conf_raw_t conf_raw_in(const conf_table_t *tab, const char *key);
CONF_EXTERN conf_raw_t conf_raw_at(const conf_array_t *arr, int idx);
CONF_EXTERN int conf_rtos(conf_raw_t s, char **ret);
CONF_EXTERN int conf_rtob(conf_raw_t s, int *ret);
CONF_EXTERN int conf_rtoi(conf_raw_t s, int64_t *ret);
CONF_EXTERN int conf_rtod(conf_raw_t s, double *ret);
CONF_EXTERN int conf_rtod_ex(conf_raw_t s, double *ret, char *buf, int buflen);
CONF_EXTERN int conf_rtots(conf_raw_t s, conf_timestamp_t *ret);

#endif  //!__CONF__H__