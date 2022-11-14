#if !defined(__COMPONENTS_STRING_INCLUDE_ALLOCATEUTILS_H__)
#define __COMPONENTS_STRING_INCLUDE_ALLOCATEUTILS_H__



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <PublicUtils.h>

#define allocate_ALIGNMENT 32
#define allocate_PAGE_SIZE 4096
#define allocate_MAX_ALLOC_FROM_POOL (allocate_PAGE_SIZE - 1)

//为了字节对齐
#define allocate_align(n, alignment) (((n) + (alignment - 1)) & ~(alignment - 1))
#define allocate_align_ptr(p, alignment) (void *)((((size_t)p) + (alignment - 1)) & ~(alignment - 1))

typedef struct AllocateUtils_node_s AllocateUtils_node_t;
typedef struct AllocateUtils_large_s AllocateUtils_large_t;
typedef struct AllocateUtils_pool_s AllocateUtils_t;

//存储大块内存的指针
struct AllocateUtils_large_s
{
	AllocateUtils_large_t *next;
	void *alloc;
};

//存储小节点的结构
struct AllocateUtils_node_s
{
	unsigned char *last;
	unsigned char *end;
	AllocateUtils_node_t *next;
	size_t failed;
};


//定义管理节点的结构 柔性数组，实际的内存是head[0]
struct AllocateUtils_pool_s
{
	size_t max;
	AllocateUtils_node_t *current;
	AllocateUtils_large_t *large;
	AllocateUtils_node_t head[0];
};






AllocateUtils_t *AllocateUtils_initializing(AllocateUtils_t **pool, size_t size);
void AllocateUtils_cleanup(AllocateUtils_t *pool);

void *AllocateUtils_pool(AllocateUtils_t *AllocateUtils, size_t size);
ok_t AllocateUtils_toString(char **pointer, AllocateUtils_t *AllocateUtils, char *data, int length);

int AllocateUtils_free(AllocateUtils_t *AllocateUtils, void *pointer);
#endif