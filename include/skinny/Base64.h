#ifndef SKINNY__BASE64__H__
#define SKINNY__BASE64__H__
#include <stddef.h>
void Base64Encode(const unsigned char *buffer,
                  size_t length,
                  char **base64Text);
void Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);
#endif //! SKINNY__BASE64__H__