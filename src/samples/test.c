#include <HashMap.h>

// 创建带头结点的链式栈
void HashMap_Create(HashMap **head)
{
    *head = (HashMap *)malloc(sizeof(HashMap));
    if (*head == NULL)
    {
        printf("CreateLinkedStack fail:malloc error\n");
        return;
    }
    (*head)->argument = NULL;
    (*head)->next = NULL;
}

// 清空栈
void HashMap_Clear(HashMap *head)
{
    HashMap *pDel;
    pDel = head->next;
    while (pDel != NULL)
    {
        head->next = pDel->next;
        HashMap_Argument_Delete(pDel->argument);
        pDel = head->next; // 从头结点获取下一个要释放的结点
    }
}

// 判断栈是否为空
int HashMap_isEmpty(HashMap *head)
{
    if (head->next == NULL)
    {
        printf("栈为空\n");
        return 1;
    }
    return 0;
}

// 入栈
void HashMap_Push(HashMap *head, HashMap_Argument *argument)
{
    if (!argument)
    {
        return;
    }
    HashMap *pInsert;
    pInsert = (HashMap *)malloc(sizeof(HashMap));
    if (pInsert == NULL)
    {
        printf("PushLinkedStack fail:malloc error\n");
        return;
    }
    pInsert->argument = argument;
    pInsert->next = head->next; // 单链表头插法
    head->next = pInsert;
}

// 出栈
void HashMap_Pop(HashMap *head, HashMap_Argument **argument)
{
    HashMap *pNode;
    if (HashMap_isEmpty(head))
    {
        return;
    }
    pNode = head->next;
    *argument = pNode->argument;
    head->next = pNode->next; // 将出栈的结点的下一个结点连接上头结点
    free(pNode);              // 释放出栈结点，防止内存泄漏
    pNode = NULL;             // 指向NULL，防止野指针
}

// 获取栈顶元素HashMap_element_top
void HashMapop(HashMap *head, HashMap_Argument **argument)
{
    if (HashMap_isEmpty(head))
    {
        return;
    }
    *argument = head->next->argument; // 头结点的下一个结点为栈顶结点
}

// 获取栈大小
int HashMap_Count(HashMap *head)
{
    int size = 0;
    HashMap *pMove;
    pMove = head->next;
    while (pMove != NULL)
    {
        size++;
        pMove = pMove->next;
    }
    return size;
}

// 打印栈元素
void HashMap_Print(HashMap *head)
{
    HashMap *pMove;
    pMove = head->next;
    while (pMove != NULL)
    {
        if (pMove->argument->type == STRING)
        {
            printf("\tname:%s {%s}\r\n", pMove->argument->name, pMove->argument->data);
        }
        else if (pMove->argument->type == INTEGER)
        {
            printf("\tname:%s {%d}\r\n", pMove->argument->name, pMove->argument->data);
        }

        pMove = pMove->next;
    }
    printf("\n");
}
int items_by_name(const void *e1, const void *e2)
{
    return strcmp(((HashMap_Argument *)e1)->name, ((HashMap_Argument *)e2)->name);
}

HashMap_Argument *HashMap_Argument_Sort_create(HashMap_Argument **arguments, HashMap *head)
{
    int count = HashMap_Count(head);
    *arguments = malloc(sizeof(HashMap_Argument) * count);
    memset(*arguments, 0x00, sizeof(HashMap_Argument) * count);
    HashMap *pMove;
    pMove = head->next;
    int i = 0;
    while (pMove != NULL)
    {
        (*arguments)[i++] = *pMove->argument;
        pMove = pMove->next;
    }

    qsort((*arguments), count, sizeof((*arguments)[0]), items_by_name);
    return *arguments;
}
// 测试链式栈
void main()
{
    HashMap_Argument *argument;
    HashMap *head;
    // 创建链式栈
    int value;
    HashMap_Create(&head);
    int d = 1;
    // 入栈

    HashMap_AddString(head, "wangxiaoli", "wangyonglin", 11);
    HashMap_AddString(head, "wangxiaoli", "wangyonglin", 11);
    HashMap_AddString(head, "wangxiaoli", "wangyonglin", 11);
    HashMap_AddInteger(head, "wangyonglin", 100);
    HashMap_AddInteger(head, "wangyonglin", 0);
    HashMap_AddInteger(head, "wangyonglin", -1);
    HashMap_AddInteger(head, "wangyonglin", -100);
   
    HashMap_Argument_Sort_create(&argument,head);
    for (size_t i = 0; i < sizeof(argument) / sizeof(argument[0]); i++)
    {
        if (argument->type == STRING)
        {
            printf("\tname:%s {%s}\r\n", argument->name, argument->data);
        }
        else if (argument->type == INTEGER)
        {
            printf("\tname:%s {%d}\r\n", argument->name, argument->data);
        }
    }
    free(argument);
    // 打印栈元素
    HashMap_Print(head);
    value = HashMap_Count(head);
    // 获取栈大小

    printf("size:%d\n", value);
    HashMap_Clear(head);
    value = HashMap_Count(head);
    printf("size:%d\n", value);
}

HashMap_Argument *HashMap_Argument_String_Create(char *name, char *data, size_t data_size)
{
    HashMap_Argument *argument = (HashMap_Argument *)malloc(sizeof(HashMap_Argument));
    if (argument)
    {
        int name_size = strlen(name);
        if (argument->name = malloc(sizeof(char) * name_size + 1))
        {
            memset(argument->name, 0x00, sizeof(char) * name_size + 1);
            memcpy(argument->name, name, name_size);
        }
        argument->type = STRING;

        if (argument->data = malloc(data_size))
        {
            memset(argument->data, 0x00, data_size);
            memcpy(argument->data, data, data_size);
        }
    }
    return argument;
}

HashMap_Argument *HashMap_Argument_Integer_Create(char *name, long data)
{
    HashMap_Argument *argument = (HashMap_Argument *)malloc(sizeof(HashMap_Argument));
    if (argument)
    {
        int name_size = strlen(name);
        if (argument->name = malloc(sizeof(char) * name_size + 1))
        {
            memset(argument->name, 0x00, sizeof(char) * name_size + 1);
            memcpy(argument->name, name, name_size);
        }
        argument->type = INTEGER;
        argument->data = (void *)data;
    }
    return argument;
}
void HashMap_Argument_Delete(HashMap_Argument *argument)
{
    if (argument)
    {
        if (argument->name)
            free(argument->name);
        if (argument->type == STRING)
        {
            if (argument->data)
                free(argument->data);
        }

        free(argument);
    }
}

void HashMap_AddString(HashMap *head, char *name, char *data, size_t data_size)
{
    HashMap_Argument *argument = HashMap_Argument_String_Create(name, data, data_size);
    if (!argument)
    {
        return;
    }
    HashMap *pInsert;
    pInsert = (HashMap *)malloc(sizeof(HashMap));
    if (pInsert == NULL)
    {
        printf("PushLinkedStack fail:malloc error\n");
        return;
    }
    pInsert->argument = argument;
    pInsert->next = head->next; // 单链表头插法
    head->next = pInsert;
}

void HashMap_AddInteger(HashMap *head, char *name, long data)
{
    HashMap_Argument *argument = HashMap_Argument_Integer_Create(name, data);
    if (!argument)
    {
        return;
    }
    HashMap *pInsert;
    pInsert = (HashMap *)malloc(sizeof(HashMap));
    if (pInsert == NULL)
    {
        printf("PushLinkedStack fail:malloc error\n");
        return;
    }
    pInsert->argument = argument;
    pInsert->next = head->next; // 单链表头插法
    head->next = pInsert;
}

void HashMap_Argument_delete(HashMap_Argument *argument){
    if(argument)free(argument);
}