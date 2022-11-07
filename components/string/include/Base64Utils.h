#if !defined(__COMPONENTS_STRING_INCLUDE_BASE64UTILS_H__)
#define __COMPONENTS_STRING_INCLUDE_BASE64UTILS_H__
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "openssl/bio.h"
#include "openssl/buffer.h"
#include "openssl/evp.h"

int Base64Utils_Base64Encode(const unsigned char *buffer,
                 size_t length,
                 char **base64Text);
void Base64Utils_Base64Decode(const char *b64message, unsigned char **buffer, size_t *length);
#endif