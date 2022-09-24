
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <errno.h>
#include "signal.h"
#include "sys/types.h"

#include "sys/stat.h"
#include "fcntl.h"
#include "dirent.h"

const char *PROC = "/proc/";
typedef struct dirent DIRENT;

int main(int argc, char **argv)
{
    printf("1\n");

    char *filename="tiger";
    DIR *dir;
    DIRENT *dirp;
    char filepath[50];
    int fd;
    int i = 0;
    int j = 0;
    int pid;
    char comm[50];
   // filename = argv[2];
    printf("filename=%s\n", filename);

    if ((dir = opendir(PROC)) == NULL)
    {
        printf("Open %s Error %s\n", PROC, strerror(errno));
        exit(1);
    }
    printf("2\n");
    while ((dirp = readdir(dir)) != NULL)
    {
        printf("dirp-name===%s\n", dirp->d_name);
        strcpy(filepath, PROC);
        strcat(filepath, dirp->d_name);
        strcat(filepath, "/status");

        if ((fd = open(filepath, O_RDONLY)) == -1)
        {
            printf("Open File %s Error:%s\n", filepath, strerror(errno));
            return 1;
        }
        printf("fd = %d\n", fd);
        for (i = 0; i < 48; i++)
        {
            read(fd, &comm[i], sizeof(char));
        }
        comm[i] = '\0';
        close(fd);

        if ((comm[6] == 'b') && (comm[7] == 'b') && (comm[8] == 'b'))
        {
            printf("==================================\n");
            pid = atoi(dirp->d_name);
            printf("pid====%d", pid);
            if (kill(pid, SIGKILL) == -1)
            {
                printf("Kill %s Error:%s\n", filename, strerror(errno));
                exit(1);
            }
            break;
        }

        printf("comm===%c%c%c\n", comm[6], comm[7], comm[8]);
        /* if(strstr(comm,filename)!=0)
            {
                        break;
            }*/
        /* while(read(fd,&comm[j],sizeof(char)))
                 {
                     if(comm[j]=='\n')
                         break;
                     j++;
                 }
        comm[j]='\0';
          close(fd);
          /* if(strcmp(comm,filename)==0)
             {

                 pid=atoi(dirp->d_name);
                 printf("pid====%d",pid);
                 break;
             }

               i++;*/
    }
    closedir(dir);
    printf("==================================");
    return 0;
}