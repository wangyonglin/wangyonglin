
#include <stdio.h>
#include <getopt.h>
char *const shortopts = "d::c:";
struct option longopts[] = {
    {"deamon", no_argument, NULL, 'd'},
    {"conf", required_argument, NULL, 'c'},
    {0, 0, 0, 0},
};
int main(int argc, char *argv[])
{
  int rc;
  while ((rc = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1)
  {
    switch (rc)
    {
    case 'd':
      if (optarg == NULL)
        printf("Run deamon %s!/n", optarg);
      break;
    case 'c':
      printf("%c: %s\n", rc, optarg);
      break;
    default:
      printf("Our love  !/n");
      break;
    }
  }
  return 0;
}
