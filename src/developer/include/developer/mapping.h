#if !defined(DEVELOPER_MAPPING_H)
#define DEVELOPER_MAPPING_H

#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <stdbool.h>
#include <developer/public.h>
#include <developer/string.h>
#define mapping_arguments_length(array) ((sizeof(array)) / (sizeof(array[0])))
typedef struct
{
    const char *name;
    void *value;
    integer type;
    size_t address;
} mapping_arguments_t;

typedef struct
{
    allocate_t *allocate;
} mapping_t;

ok_t mapping_initializing(mapping_t **mapping, allocate_t *allocate);
ok_t mapping_create(mapping_t *mapping, void *pointer, const char *filename, const char *section, mapping_arguments_t mapping_arguments[], int mappings_size);
int mapping_arguments_qsort_name(const void *e1, const void *e2);
void mapping_arguments_qsort(mapping_arguments_t *arguments,int arguments_count);
#endif