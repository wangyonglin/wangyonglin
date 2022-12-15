#include <developer/allocate.h>

// 重新申请内存块，申请大内存
static void *allocate_alloc_block(allocate_t *pool, size_t size);
static void *allocate_alloc_large(allocate_t *pool, size_t size);

// 按照字节对齐申请大块内存
void *allocate_memalign(allocate_t *pool, size_t size, size_t alignment);

// 总接口，申请大内存或者小内存，默认字节对齐方式
void *allocate_alloc(allocate_t *pool, size_t size);

// 申请内存，但是不做字节对齐处理
void *allocate_nalloc(allocate_t *pool, size_t size);
// 申请内存并重置为0
void *allocate_calloc(allocate_t *pool, size_t size);
int allocate_free(allocate_t *pool, void *p);

allocate_t *allocate_create_pool(size_t size);
void allocate_destory_pool(allocate_t *pool);
void allocate_reset_pool(allocate_t *pool);

// 申请线程池 保存管理节点+实际的内存
// 管理节点和哨兵节点
// allocate_t *allocate_create_pool(size_t size)
// {
// 	allocate_t *pool;
// 	// 申请大内存 目标申请大小+管理节点结构体
// 	// 并指定节点对齐方式
// 	int ret = posix_memalign((void **)&pool, allocate_ALIGNMENT, size + sizeof(allocate_t) + sizeof(allocate_node_t));
// 	if (ret)
// 	{
// 		printf("malloc mm pool error. \n");
// 		return NULL;
// 	}
// 	// 区分小块内存还是大块内存的标志
// 	pool->max = (size < allocate_MAX_ALLOC_FROM_POOL) ? size : allocate_MAX_ALLOC_FROM_POOL;
// 	pool->current = pool->head;
// 	pool->large = NULL;

// 	pool->head->last = (unsigned char *)pool + sizeof(allocate_t) + sizeof(allocate_node_t);
// 	pool->head->end = pool->head->last + size;
// 	pool->head->next = NULL;
// 	pool->head->failed = 0;
// 	return pool;
// }
// // 线程池的销毁处理
// void allocate_destory_pool(allocate_t *pool)
// {
// 	if (pool == NULL)
// 		return;
// 	// 先销毁大内存 保存的大块内存的起始链表节点
// 	allocate_large_t *large;
// 	large = pool->large;
// 	while (large)
// 	{
// 		if (large->alloc != NULL)
// 		{
// 			free(large->alloc);
// 		}
// 		large = large->next;
// 	}
// 	// 销毁在线程池内维护的小内存节点 可能重新分配小内存的池子吧
// 	allocate_node_t *node, *next_node;
// 	node = pool->head->next;
// 	while (node)
// 	{
// 		next_node = node->next;
// 		free(node);
// 		node = next_node;
// 	}
// 	free(pool);
// }

allocate_t *allocate_initializing(allocate_t **pool, size_t size)
{
    // allocate_t *pool;
    // 申请大内存 目标申请大小+管理节点结构体
    // 并指定节点对齐方式
    int ret = posix_memalign((void **)pool, ALLOCATE_ALIGNMENT, size + sizeof(allocate_t) + sizeof(allocate_node_t));
    if (ret)
    {
        printf("malloc mm pool error. \n");
        return NULL;
    }
    // 区分小块内存还是大块内存的标志
    (*pool)->max = (size < ALLOCATE_MAX_ALLOC_FROM_POOL) ? size : ALLOCATE_MAX_ALLOC_FROM_POOL;
    (*pool)->current = (*pool)->head;
    (*pool)->large = NULL;

    (*pool)->head->last = (unsigned char *)(*pool) + sizeof(allocate_t) + sizeof(allocate_node_t);
    (*pool)->head->end = (*pool)->head->last + size;
    (*pool)->head->next = NULL;
    (*pool)->head->failed = 0;
    return (*pool);
}

void allocate_cleanup(allocate_t *pool)
{
    if (pool == NULL)
        return;
    // 先销毁大内存 保存的大块内存的起始链表节点
    allocate_large_t *large;
    large = pool->large;
    while (large)
    {
        if (large->alloc != NULL)
        {
            free(large->alloc);
        }
        large = large->next;
    }
    // 销毁在线程池内维护的小内存节点 可能重新分配小内存的池子吧
    allocate_node_t *node, *next_node;
    node = pool->head->next;
    while (node)
    {
        next_node = node->next;
        free(node);
        node = next_node;
    }
    free(pool);
}

// 销毁大内存的申请 小块内存的重新指向
void allocate_reset_pool(allocate_t *pool)
{
    allocate_large_t *large;
    for (large = pool->large; large; large = large->next)
    {
        if (large->alloc)
        {
            free(large->alloc);
        }
    }
    pool->large = NULL;

    allocate_node_t *node;
    for (node = pool->head; node; node = node->next)
    {
        node->last = (unsigned char *)node + sizeof(allocate_node_t);
    }
}
// 按块分配内存  内存池不够用了，再分配内存
// 返回当前内存
static void *allocate_alloc_block(allocate_t *pool, size_t size)
{
    // 计算第一块内存池的大小 不需要管理结构体
    unsigned char *temp;
    allocate_node_t *node = pool->head;
    size_t psize = (size_t)(node->end - (unsigned char *)node);

    int ret = posix_memalign((void **)&temp, ALLOCATE_ALIGNMENT, psize);
    if (ret)
        return NULL;

    // 对申请到的内存连接
    allocate_node_t *new_node;
    new_node = (allocate_node_t *)temp;
    new_node->end = temp + psize;
    new_node->next = NULL;
    new_node->failed = 0;

    // 做一下字节对齐处理
    temp = temp + sizeof(allocate_node_t);
    temp = allocate_align_ptr(temp, ALLOCATE_ALIGNMENT);
    new_node->last = temp + size; // 指向已经用过后的最新空闲节点

    // 遍历当前的池子   池子申请内存大于4次就看下一个池子
    allocate_node_t *current, *p;
    current = pool->current;
    // 遍历所有的池子 有池子可用 取出来
    //  failed 统一加1，并取失败小于4次的
    for (p = current; p->next; p = p->next)
    {
        if (p->failed++ > 4)
        {
            current = p->next;
        }
    }
    // 新内存加入到池子队列中
    p->next = new_node;
    // 重新定义当前的池子
    pool->current = current ? current : new_node;
    return temp;
}

// 申请大块内存 然后用头插法插入管理节点  或者查找已经释放的大内存直接使用
static void *allocate_alloc_large(allocate_t *pool, size_t size)
{
    void *temp = malloc(size);
    if (temp == NULL)
        return NULL;

    size_t n = 0;
    allocate_large_t *large;
    for (large = pool->large; large; large = large->next)
    {
        if (large->alloc == NULL)
        {
            large->alloc = temp;
            return temp;
        }
        if (n++ > 3)
            break;
    }
    // 在小块的内存池中 申请结构体
    large = allocate_alloc(pool, sizeof(allocate_large_t));
    if (large == NULL)
    {
        free(temp);
        return NULL;
    }
    // 头插法
    large->alloc = temp;
    large->next = pool->large;
    pool->large = large;
    return temp;
}
// 按照字节对齐分配大块内存
void *allocate_memalign(allocate_t *pool, size_t size, size_t alignment)
{
    void *temp;
    int ret = posix_memalign(&temp, alignment, size);
    if (ret)
        return NULL;

    // 在内存池中取内存存结构
    allocate_large_t *large = allocate_alloc(pool, sizeof(allocate_large_t));
    if (large == NULL)
    {
        free(temp);
        return NULL;
    }
    large->alloc = temp;
    large->next = pool->large;
    pool->large = large;
    return temp;
}

// // 在内存池中申请内存
// void *allocate_pool(allocate_t *pool, size_t size)
// {
//     unsigned char *temp;
//     // 从当前节点开始  计算可用池子，申请内存
//     allocate_node_t *p;
//     if (size <= pool->max)
//     {
//         p = pool->current;
//         do
//         {
//             temp = allocate_align_ptr(p->last, allocate_ALIGNMENT);
//             if ((size_t)(p->end - temp) >= size)
//             {
//                 p->last = temp + size;
//                 return temp;
//             }
//             p = p->next;
//         } while (p);
//         // 内存不够的情况下  需要重新申请内存块
//         return allocate_alloc_block(pool, size);
//     }
//     // 申请大块内存
//     return allocate_alloc_large(pool, size);
// }
// 这个函数用于不用字节对齐的处理
void *allocate_nalloc(allocate_t *pool, size_t size)
{
    unsigned char *temp;
    allocate_node_t *p;
    if (size <= pool->max)
    {
        p = pool->current;
        do
        {
            temp = p->last;
            if ((size_t)(p->end - temp) >= size)
            {
                p->last = temp + size;
                return temp;
            }
            p = p->next;
        } while (p);
        return allocate_alloc_block(pool, size);
    }
    return allocate_alloc_large(pool, size);
}
// 在内存池中申请内存
void *allocate_alloc(allocate_t *pool, size_t size)
{
    unsigned char *temp;
    // 从当前节点开始  计算可用池子，申请内存
    allocate_node_t *p;
    if (size <= pool->max)
    {
        p = pool->current;
        do
        {
            temp = allocate_align_ptr(p->last, ALLOCATE_ALIGNMENT);
            if ((size_t)(p->end - temp) >= size)
            {
                p->last = temp + size;
                return temp;
            }
            p = p->next;
        } while (p);
        // 内存不够的情况下  需要重新申请内存块
        return allocate_alloc_block(pool, size);
    }
    // 申请大块内存
    return allocate_alloc_large(pool, size);
}
// 申请内存并重置为0
void *allocate_calloc(allocate_t *pool, size_t size)
{
    void *p = allocate_alloc(pool, size);
    if (p)
    {
        memset(p, 0, size);
    }
    return p;
}
// 释放一块大内存
int allocate_free(allocate_t *pool, void *p)
{
    allocate_large_t *large;
    for (large = pool->large; large; large = large->next)
    {
        if (p == large->alloc)
        {
            free(large->alloc);
            large->alloc = NULL;
            return 0;
        }
    }
    return -1;
}

// 线程池的销毁处理

void *allocate_create(allocate_t *allocate, size_t size)
{
    unsigned char *temp;
    // 从当前节点开始  计算可用池子，申请内存
    allocate_node_t *p;
    if (size <= allocate->max)
    {
        p = allocate->current;
        do
        {
            temp = allocate_align_ptr(p->last, ALLOCATE_ALIGNMENT);
            if ((size_t)(p->end - temp) >= size)
            {
                p->last = temp + size;
                return temp;
            }
            p = p->next;
        } while (p);
        // 内存不够的情况下  需要重新申请内存块
        return allocate_alloc_block(allocate, size);
    }
    // 申请大块内存
    return allocate_alloc_large(allocate, size);
}
int allocate_delete(allocate_t *allocate, void *pointer)
{
    allocate_large_t *large;
    for (large = allocate->large; large; large = large->next)
    {
        if (pointer == large->alloc)
        {
            free(large->alloc);
            large->alloc = NULL;
            return 0;
        }
    }
    return -1;
}

/**wangyonglin*/

ok_t string_create(allocate_t *allocate, unsigned char **pointer, char *data, size_t data_size)
{
    if (!allocate)
    {
        return ArgumentException;
    }
    if (!((*pointer) = allocate_create(allocate, sizeof(char) * data_size + 1)))
    {
        return NullPointerException;
    }
    memset((*pointer), 0x00, sizeof(char) * data_size + 1);
    memcpy((*pointer), data, data_size);
    return Ok;
}
ok_t string_delete(allocate_t *allocate, unsigned char *pointer)
{
    if (!allocate && !pointer)
    {
        return ArgumentException;
    }
    allocate_delete(allocate, pointer);
    return Ok;
}
ok_t object_create(allocate_t *allocate, void **object, size_t object_size)
{
    if (!allocate)
    {
        return ArgumentException;
    }
    if (!((*object) = allocate_create(allocate, object_size)))
    {
        return NullPointerException;
    }
    memset((*object), 0x00, object_size);
    return Ok;
}
ok_t object_delete(allocate_t *allocate, void *object)
{
    if (!allocate && !object)
    {
        return ArgumentException;
    }

    allocate_delete(allocate, object);
    return Ok;
}

void boolean_create(unsigned char **pointer, boolean value)
{
    *pointer = value;

  
}
void number_create(unsigned char **pointer, long value)
{
    *pointer = (unsigned char *)value;
}

unsigned char **touchar(void **pointer)
{
    return (unsigned char **)pointer;
}