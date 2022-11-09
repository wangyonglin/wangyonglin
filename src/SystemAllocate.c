#include <SystemAllocate.h>

// 申请线程池 保存管理节点+实际的内存
// 管理节点和哨兵节点
SystemAllocate_t *allocate_create_pool(size_t size)
{
	SystemAllocate_t *pool;
	// 申请大内存 目标申请大小+管理节点结构体
	// 并指定节点对齐方式
	int ret = posix_memalign((void **)&pool, allocate_ALIGNMENT, size + sizeof(SystemAllocate_t) + sizeof(SystemAllocate_node_t));
	if (ret)
	{
		printf("malloc mm pool error. \n");
		return NULL;
	}
	// 区分小块内存还是大块内存的标志
	pool->max = (size < allocate_MAX_ALLOC_FROM_POOL) ? size : allocate_MAX_ALLOC_FROM_POOL;
	pool->current = pool->head;
	pool->large = NULL;

	pool->head->last = (unsigned char *)pool + sizeof(SystemAllocate_t) + sizeof(SystemAllocate_node_t);
	pool->head->end = pool->head->last + size;
	pool->head->next = NULL;
	pool->head->failed = 0;
	return pool;
}

SystemAllocate_t *SystemAllocate_initializing(SystemAllocate_t **pool, size_t size)
{
	// SystemAllocate_t *pool;
	// 申请大内存 目标申请大小+管理节点结构体
	// 并指定节点对齐方式
	int ret = posix_memalign((void **)pool, allocate_ALIGNMENT, size + sizeof(SystemAllocate_t) + sizeof(SystemAllocate_node_t));
	if (ret)
	{
		printf("malloc mm pool error. \n");
		return NULL;
	}
	// 区分小块内存还是大块内存的标志
	(*pool)->max = (size < allocate_MAX_ALLOC_FROM_POOL) ? size : allocate_MAX_ALLOC_FROM_POOL;
	(*pool)->current = (*pool)->head;
	(*pool)->large = NULL;

	(*pool)->head->last = (unsigned char *)(*pool) + sizeof(SystemAllocate_t) + sizeof(SystemAllocate_node_t);
	(*pool)->head->end = (*pool)->head->last + size;
	(*pool)->head->next = NULL;
	(*pool)->head->failed = 0;
	return (*pool);
}

// 线程池的销毁处理
void allocate_destory_pool(SystemAllocate_t *pool)
{
	if (pool == NULL)
		return;
	// 先销毁大内存 保存的大块内存的起始链表节点
	SystemAllocate_large_t *large;
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
	SystemAllocate_node_t *node, *next_node;
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
void allocate_reset_pool(SystemAllocate_t *pool)
{
	SystemAllocate_large_t *large;
	for (large = pool->large; large; large = large->next)
	{
		if (large->alloc)
		{
			free(large->alloc);
		}
	}
	pool->large = NULL;

	SystemAllocate_node_t *node;
	for (node = pool->head; node; node = node->next)
	{
		node->last = (unsigned char *)node + sizeof(SystemAllocate_node_t);
	}
}
// 按块分配内存  内存池不够用了，再分配内存
// 返回当前内存
static void *allocate_alloc_block(SystemAllocate_t *pool, size_t size)
{
	// 计算第一块内存池的大小 不需要管理结构体
	unsigned char *temp;
	SystemAllocate_node_t *node = pool->head;
	size_t psize = (size_t)(node->end - (unsigned char *)node);

	int ret = posix_memalign((void **)&temp, allocate_ALIGNMENT, psize);
	if (ret)
		return NULL;

	// 对申请到的内存连接
	SystemAllocate_node_t *new_node;
	new_node = (SystemAllocate_node_t *)temp;
	new_node->end = temp + psize;
	new_node->next = NULL;
	new_node->failed = 0;

	// 做一下字节对齐处理
	temp = temp + sizeof(SystemAllocate_node_t);
	temp = allocate_align_ptr(temp, allocate_ALIGNMENT);
	new_node->last = temp + size; // 指向已经用过后的最新空闲节点

	// 遍历当前的池子   池子申请内存大于4次就看下一个池子
	SystemAllocate_node_t *current, *p;
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
static void *allocate_alloc_large(SystemAllocate_t *pool, size_t size)
{
	void *temp = malloc(size);
	if (temp == NULL)
		return NULL;

	size_t n = 0;
	SystemAllocate_large_t *large;
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
	large = allocate_alloc(pool, sizeof(SystemAllocate_large_t));
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
void *allocate_memalign(SystemAllocate_t *pool, size_t size, size_t alignment)
{
	void *temp;
	int ret = posix_memalign(&temp, alignment, size);
	if (ret)
		return NULL;

	// 在内存池中取内存存结构
	SystemAllocate_large_t *large = allocate_alloc(pool, sizeof(SystemAllocate_large_t));
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

// 在内存池中申请内存
void *allocate_alloc(SystemAllocate_t *pool, size_t size)
{
	unsigned char *temp;
	// 从当前节点开始  计算可用池子，申请内存
	SystemAllocate_node_t *p;
	if (size <= pool->max)
	{
		p = pool->current;
		do
		{
			temp = allocate_align_ptr(p->last, allocate_ALIGNMENT);
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
// 这个函数用于不用字节对齐的处理
void *allocate_nalloc(SystemAllocate_t *pool, size_t size)
{
	unsigned char *temp;
	SystemAllocate_node_t *p;
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
// 申请内存并重置为0
void *allocate_calloc(SystemAllocate_t *pool, size_t size)
{
	void *p = allocate_alloc(pool, size);
	if (p)
	{
		memset(p, 0, size);
	}
	return p;
}
// 释放一块大内存
int allocate_free(SystemAllocate_t *pool, void *p)
{
	SystemAllocate_large_t *large;
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
void SystemDeallocate_cleanup(SystemAllocate_t *pool)
{
	if (pool == NULL)
		return;
	// 先销毁大内存 保存的大块内存的起始链表节点
	SystemAllocate_large_t *large;
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
	SystemAllocate_node_t *node, *next_node;
	node = pool->head->next;
	while (node)
	{
		next_node = node->next;
		free(node);
		node = next_node;
	}
	free(pool);
}

void *SystemAllocate_Create(SystemAllocate_t *SystemAllocate, size_t size)
{
	unsigned char *temp;
	// 从当前节点开始  计算可用池子，申请内存
	SystemAllocate_node_t *p;
	if (size <= SystemAllocate->max)
	{
		p = SystemAllocate->current;
		do
		{
			temp = allocate_align_ptr(p->last, allocate_ALIGNMENT);
			if ((size_t)(p->end - temp) >= size)
			{
				p->last = temp + size;
				return temp;
			}
			p = p->next;
		} while (p);
		// 内存不够的情况下  需要重新申请内存块
		return allocate_alloc_block(SystemAllocate, size);
	}
	// 申请大块内存
	return allocate_alloc_large(SystemAllocate, size);
}
int SystemAllocate_Delect(SystemAllocate_t *SystemAllocate, void *pointer)
{
	SystemAllocate_large_t *large;
	for (large = SystemAllocate->large; large; large = large->next)
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

ok_t SystemAllocate_String(char **pointer, SystemAllocate_t *SystemAllocate,char * data, int length)
{
	if(!SystemAllocate && !data){
		return ArgumentException;
	}
	if (((*pointer) = SystemAllocate_Create(SystemAllocate, length + 1)) == NULL)
	{
		return NullPointerException;
	}
	if (memset((*pointer), 0x00, length + 1) == NULL)
	{
		return ErrorException;
	}
	if (memcpy((*pointer), data, length) == NULL)
	{
		return ErrorException;
	}
	return OK;
}