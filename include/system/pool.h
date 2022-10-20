#if !defined(__SYSTEM_POOL_H__)
#define __SYSTEM_POOL_H__
/*************************************
实现内存池的逻辑：
	1：定义大块内存的申请结构
	2：定义小块内存的申请结构
	3：定义管理节点
相关函数的实现：
	1：大块内存直接申请，保存指针
	2：小块内存使用结构进行申请
	3：大块内存和小块内存的释放逻辑
*************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <fcntl.h>
#define allocate_ALIGNMENT 32
#define allocate_PAGE_SIZE 4096
#define allocate_MAX_ALLOC_FROM_POOL (allocate_PAGE_SIZE - 1)

//为了字节对齐
#define allocate_align(n, alignment) (((n) + (alignment - 1)) & ~(alignment - 1))
#define allocate_align_ptr(p, alignment) (void *)((((size_t)p) + (alignment - 1)) & ~(alignment - 1))

typedef struct allocate_node_s allocate_node_t;
typedef struct allocate_large_s allocate_large_t;
typedef struct allocate_pool_s allocate_pool_t;

//存储大块内存的指针
struct allocate_large_s
{
	allocate_large_t *next;
	void *alloc;
};

//存储小节点的结构
struct allocate_node_s
{
	unsigned char *last;
	unsigned char *end;
	allocate_node_t *next;
	size_t failed;
};


//定义管理节点的结构 柔性数组，实际的内存是head[0]
struct allocate_pool_s
{
	size_t max;
	allocate_node_t *current;
	allocate_large_t *large;
	allocate_node_t head[0];
};

//重新申请内存块，申请大内存
static void *allocate_alloc_block(allocate_pool_t *pool, size_t size);
static void *allocate_alloc_large(allocate_pool_t *pool, size_t size);

//按照字节对齐申请大块内存
void *allocate_memalign(allocate_pool_t *pool, size_t size, size_t alignment);

//总接口，申请大内存或者小内存，默认字节对齐方式
void *allocate_alloc(allocate_pool_t *pool, size_t size);

//申请内存，但是不做字节对齐处理
void *allocate_nalloc(allocate_pool_t *pool, size_t size);
//申请内存并重置为0
void *allocate_calloc(allocate_pool_t *pool, size_t size);
int allocate_free(allocate_pool_t *pool, void *p);

allocate_pool_t *allocate_create_pool(size_t size);
void allocate_destory_pool(allocate_pool_t *pool);
void allocate_reset_pool(allocate_pool_t *pool);




allocate_pool_t *system_allocate_initializing(allocate_pool_t **pool, size_t size);
void system_deallocate_cleanup(allocate_pool_t *pool);
void *system_allocate_create(allocate_pool_t *pool, size_t size);
int system_deallocate_delect(allocate_pool_t *pool, void *p);
#endif