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
#define MP_ALIGNMENT 32
#define MP_PAGE_SIZE 4096
#define MP_MAX_ALLOC_FROM_POOL (MP_PAGE_SIZE - 1)

//为了字节对齐
#define mp_align(n, alignment) (((n) + (alignment - 1)) & ~(alignment - 1))
#define mp_align_ptr(p, alignment) (void *)((((size_t)p) + (alignment - 1)) & ~(alignment - 1))

typedef struct mp_node_s mp_node_t;
typedef struct mp_large_s mp_large_t;
typedef struct mp_pool_s mp_pool_t;

//存储大块内存的指针
struct mp_large_s
{
	mp_large_t *next;
	void *alloc;
};

//存储小节点的结构
struct mp_node_s
{
	unsigned char *last;
	unsigned char *end;
	mp_node_t *next;
	size_t failed;
};


//定义管理节点的结构 柔性数组，实际的内存是head[0]
struct mp_pool_s
{
	size_t max;
	mp_node_t *current;
	mp_large_t *large;
	mp_node_t head[0];
};

//重新申请内存块，申请大内存
static void *mp_alloc_block(mp_pool_t *pool, size_t size);
static void *mp_alloc_large(mp_pool_t *pool, size_t size);

//按照字节对齐申请大块内存
void *mp_memalign(mp_pool_t *pool, size_t size, size_t alignment);

//总接口，申请大内存或者小内存，默认字节对齐方式
void *mp_alloc(mp_pool_t *pool, size_t size);

//申请内存，但是不做字节对齐处理
void *mp_nalloc(mp_pool_t *pool, size_t size);
//申请内存并重置为0
void *mp_calloc(mp_pool_t *pool, size_t size);
int mp_free(mp_pool_t *pool, void *p);

mp_pool_t *mp_create_pool(size_t size);
void mp_destory_pool(mp_pool_t *pool);
void mp_reset_pool(mp_pool_t *pool);
#endif