#include <wangyonglin/pool.h>

// int main()
// {
// 	int size = 1 << 12; //左移12位，申请内存块的大小，一般是2的倍数 这里是2的12次方
// 	printf("pool size is : %d \n", size);
// 	pool_t *pool = allocate_create(size);
// 	printf("pool->nax = %ld \n", pool->max);
// 	//申请小块内存测试
// 	for(int i=0; i<10; i++)
// 	{
// 		void * mp = allocate(pool, 512);
// 		// deallocate(pool, mp); //这是小块内存  不用释放
// 	}
// 	void * large_t = allocate(pool, 8192);
// 	printf("large test %p \n", large_t);
// 	printf("pool large test %p \n", pool->large->alloc);

// 	printf("__align(123, 32): %d, __align(17, 32): %d\n", __align(24, 32), __align(17, 32));
// 	// printf("__align_ptr(p->current, 32): %lx, p->current: %lx, __align(p->large, 32): %lx, p->large: %lx\n", __align_ptr(p->current, 32), p->current, __align_ptr(p->large, 32), p->large);

// 	int i=0;
// 	int j = 0;
// 	for (i = 0;i < 5;i ++) {

// 		char *pp = allocate_nalloc(pool, 32);
// 		for (j = 0;j < 32;j ++) {
// 			if (pp[j]) {
// 				printf("calloc wrong\n");
// 			}
// 			// printf("calloc success\n");
// 		}
// 	}

// 	allocate_reset(pool);

// 	for (i = 0;i < 58;i ++) {
// 		allocate(pool, 256);
// 	}

// 	allocate_delete(pool);
// 	return 0;
// }

// 申请线程池 保存管理节点+实际的内存
// 管理节点和哨兵节点
pool_t *allocate_create(size_t size)
{
	pool_t *pool;
	// 申请大内存 目标申请大小+管理节点结构体
	// 并指定节点对齐方式
	int ret = posix_memalign((void **)&pool, POOL_ALIGNMENT, size + sizeof(pool_t) + sizeof(node_t));
	if (ret)
	{
		printf("malloc mm pool error. \n");
		return NULL;
	}
	// 区分小块内存还是大块内存的标志
	pool->max = (size < POOL_MAX_ALLOC_FROM_POOL) ? size : POOL_MAX_ALLOC_FROM_POOL;
	pool->current = pool->head;
	pool->large = NULL;

	pool->head->last = (unsigned char *)pool + sizeof(pool_t) + sizeof(node_t);
	pool->head->end = pool->head->last + size;
	pool->head->next = NULL;
	pool->head->failed = 0;
	return pool;
}

// 线程池的销毁处理
void allocate_delete(pool_t *pool)
{
	if (pool == NULL)
		return;
	// 先销毁大内存 保存的大块内存的起始链表节点
	large_t *large;
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
	node_t *node, *next_node;
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
void allocate_reset(pool_t *pool)
{
	large_t *large;
	for (large = pool->large; large; large = large->next)
	{
		if (large->alloc)
		{
			free(large->alloc);
		}
	}
	pool->large = NULL;

	node_t *node;
	for (node = pool->head; node; node = node->next)
	{
		node->last = (unsigned char *)node + sizeof(node_t);
	}
}
// 按块分配内存  内存池不够用了，再分配内存
// 返回当前内存
static void *allocate_block(pool_t *pool, size_t size)
{
	// 计算第一块内存池的大小 不需要管理结构体
	unsigned char *temp;
	node_t *node = pool->head;
	size_t psize = (size_t)(node->end - (unsigned char *)node);

	int ret = posix_memalign((void **)&temp, POOL_ALIGNMENT, psize);
	if (ret)
		return NULL;

	// 对申请到的内存连接
	node_t *new_node;
	new_node = (node_t *)temp;
	new_node->end = temp + psize;
	new_node->next = NULL;
	new_node->failed = 0;

	// 做一下字节对齐处理
	temp = temp + sizeof(node_t);
	temp = __align_ptr(temp, POOL_ALIGNMENT);
	new_node->last = temp + size; // 指向已经用过后的最新空闲节点

	// 遍历当前的池子   池子申请内存大于4次就看下一个池子
	node_t *current, *p;
	current = pool->current;
	// 遍历所有的池子 有池子可用 取出来
	// failed 统一加1，并取失败小于4次的
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
static void *allocate_large(pool_t *pool, size_t size)
{
	void *temp = malloc(size);
	if (temp == NULL)
		return NULL;

	size_t n = 0;
	large_t *large;
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
	large = allocate(pool, sizeof(large_t));
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
void *allocate_memalign(pool_t *pool, size_t size, size_t alignment)
{
	void *temp;
	int ret = posix_memalign(&temp, alignment, size);
	if (ret)
		return NULL;

	// 在内存池中取内存存结构
	large_t *large = allocate(pool, sizeof(large_t));
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
void *allocate(pool_t *pool, size_t size)
{
	unsigned char *temp;
	// 从当前节点开始  计算可用池子，申请内存
	node_t *p;
	if (size <= pool->max)
	{
		p = pool->current;
		do
		{
			temp = __align_ptr(p->last, POOL_ALIGNMENT);
			if ((size_t)(p->end - temp) >= size)
			{
				p->last = temp + size;
				return temp;
			}
			p = p->next;
		} while (p);
		// 内存不够的情况下  需要重新申请内存块
		return allocate_block(pool, size);
	}
	// 申请大块内存
	return allocate_large(pool, size);
}
// 这个函数用于不用字节对齐的处理
void *allocate_nalloc(pool_t *pool, size_t size)
{
	unsigned char *temp;
	node_t *p;
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
		return allocate_block(pool, size);
	}
	return allocate_large(pool, size);
}
// 申请内存并重置为0
void *allocate_calloc(pool_t *pool, size_t size)
{
	void *p = allocate(pool, size);
	if (p)
	{
		memset(p, 0, size);
	}
	return p;
}
// 释放一块大内存
int deallocate(pool_t *pool, void *p)
{
	large_t *large;
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

void *object_create(pool_t *pool, void **obj, size_t objsize)
{
	if (!pool)
	{
		return NULL;
	}
	if (!((*obj) = allocate(pool, objsize)))
	{
		return NULL;
	}
	return *obj;
}
char *string_create(pool_t *pool, char **obj, char *text, size_t textsize)
{
	if (!pool)
	{
		return NULL;
	}
	if (!((*obj) = allocate(pool, textsize+1)))
	{
		return NULL;
	}
	memset(*obj,0x00,textsize+1);
	return memcpy(*obj,text,textsize);
}