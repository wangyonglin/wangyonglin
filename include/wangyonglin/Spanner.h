#ifndef __SPANNER__H__
#define __SPANNER__H__
#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/core.h>
int timestamp(char **dest);
int randhex(char **dest, int bits);
int ciphertext(char **dest,char *buffer, int length);
#endif  //!__SPANNER__H__