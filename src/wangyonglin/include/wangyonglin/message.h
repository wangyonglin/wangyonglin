#if !defined(INCLUDE_DEVELOPER_MESSAGE_H)
#define INCLUDE_DEVELOPER_MESSAGE_H
#include <unistd.h>
#include <stdio.h>
#include <getopt.h> //getopt_long
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <malloc.h>
#include <wangyonglin/package.h>

typedef int message_id_t;
typedef struct
{
    long type;
    char data[1024];
} message_t;
message_id_t sutpc_creatMsgQue(int key);
ok_t sutpc_sndMsgQue(message_id_t id, int type, char *buffer, int length);
ok_t sutpc_blockRcvMsgQue(message_id_t id, int type, char *rcvBuff, int rcvSize);
ok_t sutpc_noBlockRcvMsgQue(message_id_t id, int type, char *rcvBuff, int rcvSize);
ok_t sutpc_deleteMsgQue(message_id_t msgid);
#endif