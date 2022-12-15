
#if !defined(INCLUDE_WANGYONGLIN_ELEMENTS_H)
#define INCLUDE_WANGYONGLIN_ELEMENTS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <developer/public.h>
#ifndef STRING
typedef int integer;
#define positive ((integer)1)
#define negative ((integer)0)

#define STRING ((integer)1)
#define INTEGER ((integer)2)
#define BOOLEAN ((integer)3)
#endif

typedef struct _element
{
    char *name;
    void *content;
    integer type;
} element;

typedef struct _elements
{
    struct _element **element_data;
    size_t element_max;
    int element_pos;
} elements;

elements *elements_create(elements **pointer,size_t element_max);
void elements_delete(elements *arguments);

ok_t elements_addstring(elements *pointer, char *name, char *content, size_t content_size);
ok_t elements_addinteger(elements *pointer, char *name, long content);
size_t elements_sort(elements *arguments, struct _element **element_date);
size_t elements_count(elements *arguments);
element *elements_object(elements *arguments, size_t arguments_index);

element *__element_string(element **pointer, char *name, char *content, size_t content_size);
element *__element_integer(element **pointer, char *name, long content);
int __elements_by_name(const void *e1, const void *e2);

#endif
