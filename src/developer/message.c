#include <developer/message.h>

message_id_t sutpc_creatMsgQue(int key)
{
    int msgid;
    if ((msgid = msgget(key, IPC_CREAT | 0777)) < 0)
    { // 第二个参数 用来确定消息队列的访问权限。返回消息队列的标识 如果这个消息队列已经存在，则返回ID
        perror("msgget error");
        ExitException;
    }

    return msgid;
}

ok_t sutpc_sndMsgQue(message_id_t id, int type, char *buffer, int length)
{
    message_t message;
    memset(&message, 0, sizeof(message_t));
    message.type = type;

    if (length > sizeof(message.data))
    {
        perror("msg length is too long\n");
        return ErrorException;
    }
    strcpy(message.data, buffer);
    if (msgsnd(id, (void *)&message, length, IPC_NOWAIT) == -1)
    {
        perror("msgsnd");
        return ErrorException;
    }
    return Ok;
}
ok_t sutpc_blockRcvMsgQue(message_id_t id, int type, char *rcvBuff, int rcvSize)
{
    int ret = 0;
    message_t message;
    memset(&message, 0, sizeof(message_t));

    ret = msgrcv(id, (void *)&message, sizeof(message.data), type, 0); // 没有指定IPC_NOWAIT，进程阻塞，挂起执行直至：有了指定类型的消息
    if (ret == -1)
    {
        perror("msgrcv");
        return ErrorException;
    }

    strcpy(rcvBuff, message.data);
    return Ok;
}
ok_t sutpc_noBlockRcvMsgQue(message_id_t id, int type, char *rcvBuff, int rcvSize)
{
    int ret = 0;
    message_t message;
    memset(&message, 0, sizeof(message_t));

    ret = msgrcv(id, (void *)&message, rcvSize, type, IPC_NOWAIT); // 不阻塞
    if (ret == -1)
    {
        perror("msgrcv");
        return ErrorException;
    }

    strcpy(rcvBuff, message.data);
    return Ok;
}

ok_t sutpc_deleteMsgQue(message_id_t msgid)
{

    if (msgctl(msgid, IPC_RMID, NULL) == -1)
    {
        perror("msgctl");
        return ErrorException;
    }
    return Ok;
}