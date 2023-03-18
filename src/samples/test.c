#include <stdio.h>
#include <time.h>
#include <string.h>

int main(void)
{
	char log_line[32] = {0};
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	strftime(log_line, sizeof(log_line), "%Y-%m-%d.error.log", info);
	printf("\t\r[%s->%d]\n",log_line,strlen(log_line));
	return 0;
}