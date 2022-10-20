#include "pool.h"

int main()
{
	int size = 1 << 12; //左移12位，申请内存块的大小，一般是2的倍数 这里是2的12次方
	printf("pool size is : %d \n", size);
	mp_pool_t *pool = mp_create_pool(size);
	printf("pool->nax = %ld \n", pool->max);
	//申请小块内存测试
	for (int i = 0; i < 1000000; i++)
	{
		void *mp = mp_alloc(pool, 512);
		sprintf(mp, "\r\t[%d]wangyonglin\n", i);
		printf(mp);
		mp_free(pool, mp); //这是小块内存  不用释放
	}
	void *large_t = mp_alloc(pool, 8192);
	printf("large test %p \n", large_t);
	printf("pool large test %p \n", pool->large->alloc);

	printf("mp_align(123, 32): %d, mp_align(17, 32): %d\n", mp_align(24, 32), mp_align(17, 32));
	// printf("mp_align_ptr(p->current, 32): %lx, p->current: %lx, mp_align(p->large, 32): %lx, p->large: %lx\n", mp_align_ptr(p->current, 32), p->current, mp_align_ptr(p->large, 32), p->large);

	int i = 0;
	int j = 0;
	for (i = 0; i < 5; i++)
	{

		char *pp = mp_calloc(pool, 32);
		for (j = 0; j < 32; j++)
		{
			if (pp[j])
			{
				printf("calloc wrong\n");
			}
			// printf("calloc success\n");
		}
	}

	mp_reset_pool(pool);

	for (i = 0; i < 58; i++)
	{
		mp_alloc(pool, 256);
	}

	mp_destory_pool(pool);
	return 0;
}
