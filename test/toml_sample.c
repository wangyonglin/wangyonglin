#ifdef NDEBUG
#undef NDEBUG
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include "toml.h"




/*设置输出颜色*/
#define red_color "\E[1;31m"
#define color_suffix "\E[0m"



/**
 * @brief 解析打印toml文件
 *
 * @param fp
 */
static void cat(FILE *fp)
{
  char errbuf[200];
  int i;
  const char *key;
  const char *raw;
  toml_table_t *tab = toml_parse_file(fp, errbuf, sizeof(errbuf));
  if (!tab)
  {
    fprintf(stderr, "ERROR: %s\n", errbuf);
    return;
  }

  /*如果是KV则打印*/
  if (0 != (raw = toml_raw_in(tab, "port")))
  {
    printf(red_color "如果是KV则打印\n" color_suffix);
    printf("%s = %s\n", key, raw);
    /*如果是表中数组或者KV型数组*/
  }
  toml_free(tab);
}

int main(int argc, const char *argv[])
{
  int i;
  if (argc == 1)
  {
    cat(stdin);
  }
  else
  {
    for (i = 1; i < argc; i++)
    {

      FILE *fp = fopen(argv[i], "r");
      if (!fp)
      {
        fprintf(stderr, "ERROR: cannot open %s: %s\n",
                argv[i], strerror(errno));
        exit(1);
      }
      cat(fp);
      fclose(fp);
    }
  }
  return 0;
}
