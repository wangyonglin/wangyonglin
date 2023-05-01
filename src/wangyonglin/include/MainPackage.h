#if !defined(INCLUDE_MAINPACKAGE_H)
#define INCLUDE_MAINPACKAGE_H
#include <fcntl.h>  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <getopt.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <openssl/x509v3.h>
#include <openssl/ssl.h>
#include <string.h>
#include <config.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/conf.h>
#include <openssl/bio.h>
#include <string_by_this.h>



typedef int ok_t;

#define OK ((ok_t)0)
#define DoneException ((ok_t)1)
#define NoneException ((ok_t)0)
#define NullPointerException ((ok_t)-1)
#define ErrorException ((ok_t)-2)
#define ArgumentException ((ok_t)-3)
#define UnknownException ((ok_t)-4)
#define FileNotFoundException ((ok_t)-5)
#define ExitException ((ok_t)-9)


#endif