
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <errno.h>

int main(int argc, char **argv)
{
    char *strbuffer;
    int len =sizeof(char) ;
    strbuffer = (char *)malloc(len * 10);
    memset(strbuffer, 0x00, sizeof(strbuffer));
    strncpy(strbuffer,"wangyonglin1111111wangyonglin1111111wangyonglin1111111",21);
    printf("%d -> %s -> %d -> %d\n", len,strbuffer,sizeof(strbuffer), strlen(strbuffer));
    return 0;
}