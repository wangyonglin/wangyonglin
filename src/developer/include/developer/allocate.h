#if !defined(DEVELOPER_ALLOCAE_H)
#define DEVELOPER_ALLOCAE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <developer/public.h>

#define ALLOCATE_ALIGNMENT 32
#define ALLOCATE_PAGE_SIZE 4096
#define ALLOCATE_MAX_ALLOC_FROM_POOL (ALLOCATE_PAGE_SIZE - 1)

//为了字节对齐
#define allocate_align(n, alignment) (((n) + (alignment - 1)) & ~(alignment - 1))
#define allocate_align_ptr(p, alignment) (void *)((((size_t)p) + (alignment - 1)) & ~(alignment - 1))

typedef struct allocate_node_s allocate_node_t;
typedef struct allocate_large_s allocate_large_t;
typedef struct allocate_pool_s allocate_t;

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






allocate_t *allocate_initializing(allocate_t **pool, size_t size);
void allocate_cleanup(allocate_t *pool);

void *allocate_create(allocate_t *pool, size_t size);
int allocate_delect(allocate_t *pool, void *pointer);


ok_t string_create(allocate_t *allocate,char **pointer, char *data, size_t data_size);
void string_delect(allocate_t *allocate,char *pointer);
ok_t object_create(allocate_t *allocate,void **object, size_t object_size);
void object_delect(allocate_t *allocate,void *object);
#endif