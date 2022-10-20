
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/poll.h>
#include <fcntl.h>
#include <errno.h>

typedef struct c
{
  int  cnt;								//数量
	int  usedcnt;						//使用个数
	int blocksize;						//内存块大小
	char* firstaddr;					//起始地址
	char* lastaddr;						//结束地址
	MEMBLOCK *firstblock;		//指向下一节点的指针

}palloc_pool_t;


int main(int argv, char *args[])
{

}