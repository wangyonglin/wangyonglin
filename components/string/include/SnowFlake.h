#if !defined(__STRING_SONWFLAKE_H__)
#define __STRING_SONWFLAKE_H__

/*
   snowflake

   ID 生成策略
   毫秒级时间41位+机器ID 10位+毫秒内序列12位。
   0 41 51 64 +-----------+------+------+ |time |pc |inc | +-----------+------+------+
   前41bits是以微秒为单位的timestamp。
   接着10bits是事先配置好的机器ID。
   最后12bits是累加计数器。
   macheine id(10bits)标明最多只能有1024台机器同时产生ID，sequence number(12bits)也标明1台机器1ms中最多产生4096个ID， *
     注意点，因为使用到位移运算，所以需要64位操作系统，不然生成的ID会有可能不正确
    demo
    int main(int argc, char *argv[])
    {
        SnowFlake_Workid(100);
        int size;
        for (;;)
        {
            uint64_t unquie = SnowFlake_IdGenInstance();
            printf("pthread_id:%u, id [%llu]\n",gettid(),unquie);
        }
            return EXIT_SUCCESS;
    }
    run
    pthread_id:503401, id [6990479680659144730]
    pthread_id:503401, id [6990479680659144731]
    pthread_id:503401, id [6990479680659144732]
    pthread_id:503401, id [6990479680659144733]
    pthread_id:503401, id [6990479680659144734]
    pthread_id:503401, id [6990479680659144735]
    pthread_id:503401, id [6990479680659144736]
    pthread_id:503401, id [6990479680659144737]
    pthread_id:503401, id [6990479680659144738]
    pthread_id:503401, id [6990479680659144739]
*/

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <linux/unistd.h>
#include <sys/syscall.h>
#include <errno.h>
#include <linux/types.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

struct globle
{
    int global_int : 12;
    uint64_t last_stamp;
    int workid;
    int seqid;
};

void SnowFlake_initializing(int workid);
uint64_t SnowFlake_IdGenInstance();
#endif