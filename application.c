
#include <toml/toml.h>
#include <unix/conf.h>
#include <string/string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mbedtls/base64.h>
const char *cp = "/home/wangyonglin/github/wangyonglin/conf/tiger.conf";
conf_t *conf;
int main(int args, char **argv)
{
  size_t olen;
  unsigned char dst[30];

  mbedtls_base64_encode(dst, 30, &olen, "wanyonglin", 11);
  printf(dst);
  return 0;
}