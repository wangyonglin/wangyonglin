#if !defined(DEVELOPER_ARGS_H)
#define DEVELOPER_ARGS_H
#include <unistd.h>
#include <stdio.h>
#include <getopt.h> //getopt_long
#include <string.h>
#include <developer/allocate.h>
typedef struct
{
    unsigned char *ini_filename;
    boolean started;
    boolean daemoned;
} args_t;

ok_t args_initializing(args_t **args, allocate_t *allocate, int argc, char *argv[]);
#endif