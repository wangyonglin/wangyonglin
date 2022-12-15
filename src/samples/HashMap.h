
#ifndef INCLUDE_HASHMAP_H
#define INCLUDE_HASHMAP_H

#ifdef __cplusplus
extern "C"
{
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
    typedef int integer;
#define STRING ((integer)1)
#define INTEGER ((integer)2)
#define BOOLEAN ((integer)3)
    typedef struct
    {
        char *name;
        void *data;
        integer type;
    } HashMap_Argument;

    typedef struct HashMaps
    {
        HashMap_Argument *argument;
        struct HashMaps *next;
    } HashMap;

    void HashMap_Create(HashMap **head);
    void HashMap_Clear(HashMap *head);
    int HashMap_isEmpty(HashMap *head);
    int HashMap_Count(HashMap *head);
    void HashMap_Print(HashMap *head);

    void HashMap_AddString(HashMap *head, char *name, char *data, size_t data_size);
    void HashMap_AddInteger(HashMap *head, char *name, long data);
    void HashMap_AddBoolean(HashMap *head, char *name, bool data);

    HashMap_Argument *HashMap_Argument_Sort_create(HashMap_Argument **arguments, HashMap *head);
    void HashMap_Argument_Sort_delete(HashMap_Argument *argument);
#ifdef __cplusplus
}
#endif

#endif
