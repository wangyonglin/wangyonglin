#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
void __attribute__((destructor)) __exit()
{
	printf("this is exit\n");
	exit(EXIT_SUCCESS);
}

void __attribute__((constructor)) init()
{
	printf("this is init\n");
	signal(SIGHUP, __exit); //* 下面设置三个信号的处理方法
	signal(SIGINT, __exit);
	signal(SIGALRM, __exit);
}

int main()
{
	printf("this is main\n");
	char * filename = "/home/wangyonglin/github/wangyonglin/conf";
	DIR *dir;
	struct dirent *ptr;
	int i;

	dir = opendir(filename);
	while ((ptr = readdir(dir)) != NULL)
	{

		printf("d_name: %s\n", ptr->d_name);
	}
	closedir(dir);
	return 0;
}