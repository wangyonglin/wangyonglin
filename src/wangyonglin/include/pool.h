#if !defined(INCLUDE_WANGYONGLIN_POOL_H)
#define INCLUDE_WANGYONGLIN_POOL_H

#include <MainPackage.h>
#include <string_by_this.h>

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

#define POOL_ALIGNMENT       		32
#define POOL_PAGE_SIZE			4096
#define POOL_MAX_ALLOC_FROM_POOL	(POOL_PAGE_SIZE-1)

//为了字节对齐
#define __align(n, alignment) (((n)+(alignment-1)) & ~(alignment-1))
#define __align_ptr(p, alignment) (void *)((((size_t)p)+(alignment-1)) & ~(alignment-1))

//存储大块内存的指针
typedef struct _large_t{
	struct _large_t *next;
	void * alloc;
}large_t;

//存储小节点的结构
typedef struct _node_t
{
	unsigned char * last;
	unsigned char * end;
	struct _node_t *next;
	size_t failed;
}node_t;

//定义管理节点的结构 柔性数组，实际的内存是head[0]
typedef struct _pool_t
{
	size_t max;

	struct _node_t * current;
	struct _large_t * large;

	struct _node_t head[0];
}pool_t;


//重新申请内存块，申请大内存
static void* allocate_block(pool_t * pool, size_t size);
static void* allocate_large(pool_t * pool, size_t size);

//按照字节对齐申请大块内存
void* allocate_memalign(pool_t * pool, size_t size, size_t alignment);

//总接口，申请大内存或者小内存，默认字节对齐方式
void* allocate(pool_t * pool, size_t size);

//申请内存，但是不做字节对齐处理
void * allocate_nalloc(pool_t * pool, size_t size);
//申请内存并重置为0
void * allocate_calloc(pool_t * pool, size_t size);
int deallocate(pool_t *pool, void* p);

pool_t * pool_create(size_t size);
void pool_delete(pool_t* pool);
void pool_reset(pool_t * pool);

void *pool_object_create(pool_t *pool, void **outobject, size_t outobjectsize);
//string_t pool_string_create(pool_t *pool, char *datastring, size_t datalength);
char *pool_buffer_create(pool_t *pool, char **outstring, char *instring, size_t instringsize);
#endif