#include <stdio.h>
typedef struct 
{
  char * name;
  int type;
}conf_command_t;

void bubble_sort(conf_command_t commands[], int size)
{

 
  for (int i = 0; i < size ; i++) // 排序要排 元素总个数-1，最后一个元素不用排
  {
    printf("\t%s %d \r\n", commands[i].name, commands[i].type);

  }
}
conf_command_t arr[] = {
  {"wangyonglin",11},
  {"wangxiaoli",10},
  {"wangyujie",9}
  };
int main()
{

  int i = 0;

  bubble_sort(arr,sizeof(arr)/sizeof(arr[0]));
 
  return 0;
}
