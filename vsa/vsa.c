/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 9/1/2023	               */
/* WORKSHEET NAME: Variable-Size Allocator*/
/* REVIEWER: Dotan				         */
/*--------------------------------------*/

#include <assert.h>	/* assert() */
#include <string.h>	/*memcpy()*/

#include "vsa.h"	/* function declarations and typedefs */

#define WORD_SIZE (sizeof(size_t))
#define BYTES_TO_ALIGN(reminder) ((reminder) ? (WORD_SIZE - (reminder)) : 0)
#define MASK ((size_t)1)
#define ABS(x) (((x) & MASK) ? ((x) - 1) : (x))
#define SET_TO_NOT_FREE(block, alloc_size) (block->size = (alloc_size) + 1)
#define MIN_POOL_SIZE 64

struct vsa
{
	size_t vsa_size;
};

typedef struct block
{
	size_t size;
	#ifndef NDEBUG
		vsa_t *vsa;
	#endif
} block_t;

static int IsEndOfVSA(vsa_t *vsa, block_t *block);
static void NextFreeBlock(block_t *runner, size_t free_block_size, 
							size_t alloc_size, vsa_t *vsa);
static size_t GetBlockSize(block_t *block);
static int IsBlockFree(block_t *block);
static void AlignPool(void *pool, size_t *reminder);
static block_t *GetFirstBlock(vsa_t *vsa);
static block_t *GetNextBlock(block_t *block);
static int IsDefragment(vsa_t *vsa, block_t *current, block_t *next);
#ifndef NDEBUG
static int IsValidBlock(block_t *block);
#endif

vsa_t *VSAInit(void *memory_pool, size_t pool_size)
{
	size_t reminder = 0;
	block_t *first_block = NULL;
	size_t vsa_size = pool_size - sizeof(vsa_t);
	vsa_t *vsa = NULL;
	
	assert(NULL != memory_pool);
	assert(MIN_POOL_SIZE <= pool_size);
	
	AlignPool(&memory_pool, &reminder);
	
	vsa = memory_pool;
	vsa->vsa_size = vsa_size - BYTES_TO_ALIGN(reminder);
	vsa->vsa_size -= vsa->vsa_size & (WORD_SIZE - 1);
										
	first_block = GetFirstBlock(vsa);
	first_block->size = vsa->vsa_size;
	
	#ifndef NDEBUG
		first_block->vsa = vsa;
	#endif
	
	return (vsa);
}

void *VSAAllocate(vsa_t *vsa, size_t block_size)
{
	block_t *runner = NULL;
	size_t reminder = block_size % WORD_SIZE;
	void *new_alloc = NULL;
	
	assert(NULL != vsa);
	assert(0 != block_size);
	
	runner = GetFirstBlock(vsa);
	block_size += BYTES_TO_ALIGN(reminder);
	
	while (!IsEndOfVSA(vsa, runner) && NULL == new_alloc)
	{
		size_t alloc_size = block_size + sizeof(block_t);
		block_t *next_block = GetNextBlock(runner);
		
		if (IsDefragment(vsa, runner, next_block) &&
			(GetBlockSize(runner) < alloc_size))
		{
			runner->size += GetBlockSize(next_block);
		}
		else if ((GetBlockSize(runner) >= alloc_size) && IsBlockFree(runner))
		{
			size_t free_block_size = GetBlockSize(runner);
			SET_TO_NOT_FREE(runner, alloc_size);
			new_alloc = (void *)(runner + 1);
			
			NextFreeBlock(runner, free_block_size, alloc_size, vsa);
		}
		else
		{
			runner = next_block;
		}
	}
	
	return new_alloc;
}

void VSAFree(void *block)
{
	assert(NULL != block);
	
	block = (void *)((block_t *)block - 1);
	
	assert(IsValidBlock(block));
	
	((block_t *)block)->size = GetBlockSize((block_t *)block);
}

size_t VSAGetMaxFreeBlockSize(vsa_t *vsa)
{
	size_t max_block_size = 0;
	block_t *runner = NULL;
	size_t reminder = 0;
	
	assert(NULL != vsa);
	
	runner = (block_t *)(vsa + 1);
	
	while (!IsEndOfVSA(vsa, runner))
	{
		block_t *next_block = GetNextBlock(runner);
		
		if (IsDefragment(vsa, runner, next_block))
		{
			runner->size += GetBlockSize(next_block);
		}
		else if (IsBlockFree(runner) && (GetBlockSize(runner) > max_block_size))
		{
			max_block_size = GetBlockSize(runner);
		}
		else
		{
			runner = next_block;
		}
	}
	
	reminder += sizeof(block_t);
	max_block_size = (max_block_size > reminder) ?
					 (max_block_size - reminder) : 0;
	
	return max_block_size;
}

void *VSARealloc(vsa_t *vsa, void *block, size_t new_size)
{
	block_t *next_block = NULL;
	block_t *old_block = NULL;
	long old_block_size = 0;
	size_t reminder = new_size & (WORD_SIZE - 1);
	
	assert(NULL != vsa);
	assert(NULL != block);
	assert(vsa == ((block_t *)((block_t *)block - 1))->vsa);
	
	old_block = block;
	block = (block_t *)block - 1;
	next_block = GetNextBlock(block);
	old_block_size = (GetBlockSize((block_t *)block)) - sizeof(block_t);
	new_size += BYTES_TO_ALIGN(reminder);
	
	if (!IsEndOfVSA(vsa, next_block) && IsBlockFree(next_block) &&
		((old_block_size + GetBlockSize(next_block)) > 
		(new_size + sizeof(block_t))))
	{
		size_t old_next_size = GetBlockSize(next_block);
		SET_TO_NOT_FREE(((block_t *)block), (new_size + sizeof(block_t)));
		next_block = GetNextBlock(block);
		next_block->size = old_next_size - (new_size - old_block_size);	
		#ifndef NDEBUG
			next_block->vsa = vsa;
		#endif
		
		block = (char *)block + sizeof(block_t);
	}
	else if(NULL != (block = VSAAllocate(vsa, new_size)))
	{
		memcpy(block, old_block, old_block_size);
		VSAFree(old_block);
	}
	
	return block;
}

static int IsEndOfVSA(vsa_t *vsa, block_t *block)
{
	assert(NULL != vsa);
	assert(NULL != block);
	
	return (vsa->vsa_size <= ((size_t)block - (size_t)vsa));
}

static void NextFreeBlock(block_t *runner, size_t free_block_size, 
							size_t alloc_size, vsa_t *vsa)
{
	assert(NULL != runner);
	assert(NULL != vsa);
	
	if (sizeof(block_t) < (size_t)(free_block_size - alloc_size))
	{
		runner = (block_t *)((char *)runner + alloc_size);
		runner->size = free_block_size - alloc_size;
		#ifndef NDEBUG
			runner->vsa = vsa;
		#endif
	}
	else
	{
		runner->size += free_block_size - alloc_size;
	}
	(void)vsa;
}

static size_t GetBlockSize(block_t *block)
{
	assert(NULL != block);
	
	return (ABS(block->size));
}

static int IsBlockFree(block_t *block)
{
	assert(NULL != block);
	
	return !(block->size & MASK);
}

static void AlignPool(void *pool, size_t *reminder)
{
	assert(NULL != pool);
	assert(NULL != reminder);
	
	*reminder = *(size_t *)pool & (WORD_SIZE - 1);
	*(void **)pool = *(char **)pool + BYTES_TO_ALIGN(*reminder);
}

static block_t *GetFirstBlock(vsa_t *vsa)
{
	assert(NULL != vsa);
	
	return ((block_t *)(vsa + 1));
}

static block_t *GetNextBlock(block_t *block)
{
	assert(NULL != block);
	
	return ((block_t *)((char *)block + GetBlockSize(block)));
}

static int IsDefragment(vsa_t *vsa, block_t *current, block_t *next)
{
	int status = 0;
	
	assert(NULL != vsa);
	assert(NULL != current);
	assert(NULL != next);
	
	if (!IsEndOfVSA(vsa, next))
	{
		status = 1;
		status *= IsBlockFree(current);
		status *= IsBlockFree(next);
	}
	
	return (status);
}

#ifndef NDEBUG
static int IsValidBlock(block_t *block)
{
	void *vsa = NULL;
	void *eov = NULL;
	
	assert(NULL != block);
	assert(NULL != block->vsa);
	
	vsa = block->vsa;
	eov = (void *)((size_t)vsa + ((vsa_t *)vsa)->vsa_size);
	
	return (((void *)block > vsa) && ((void *)block < eov));
}
#endif

