#ifndef __BASE64__H__
#define __BASE64__H__
#include <wangyonglin/wangyonglin.h>
#include <wangyonglin/core.h>

int Base64Encode(const unsigned char *buffer,
                  size_t length,
                  char **base64Text);
void Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);
#endif  //!__BASE64__H__