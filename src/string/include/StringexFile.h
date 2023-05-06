#if !defined(INCLUDE_STRINGEXFILE_H)
#define INCLUDE_STRINGEXFILE_H
#include <stdio.h>
#include <stdbool.h>
#include <openssl/bio.h>
#include <openssl/err.h>
#include <Stringex.h>
Integer StringexFileDataSize(FILE *file);
Boolean StringexFileDataCallback(Stringex *callStringex, char *filename);

#endif