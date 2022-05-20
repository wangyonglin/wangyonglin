#ifndef __UTILS__H__
#define __UTILS__H__

#include <wangyonglin/linux.h>

int utils_timestamp(char **dest);
int utils_randhex(char **dest, int bits);
int utils_ciphertext(char **dest,char *buffer, int length);
#endif  //!__UTILS__H__