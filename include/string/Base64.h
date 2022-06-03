#ifndef __BASE64__H__
#define __BASE64__H__

#include <wangyonglin/linux.h>


int string_base64_encode(const unsigned char *buffer,
                  size_t length,
                  char **base64Text);
void string_base64_decode(const char *b64message, unsigned char **buffer, size_t *length);
#endif  //!__BASE64__H__