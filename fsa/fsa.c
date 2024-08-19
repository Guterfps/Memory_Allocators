/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 5/1/2023	               */
/* WORKSHEET NAME: Fixed-Size Allocator   */
/* REVIEWER: Yuval Atoun		         */
/*--------------------------------------*/

#include <assert.h>	/* assert() */

#include "fsa.h"	/* function declerations and typedefs */

#define WORD_SIZE (sizeof(size_t))
#define BYTES_TO_ALIGN_UP(reminder) ((reminder) ? (WORD_SIZE - (reminder)) : 0)
#define POOL_ALIGN_UP (WORD_SIZE - 1)

struct fsa
{
	size_t free_block;
};

static void FSASetUp(void **memory_pool, size_t *block_size);
static void BlockSetUp(size_t *block_size);
static void FreeListSetUp(char *runner, size_t *offset_counter, 
							size_t block_size, size_t num_of_blocks);

size_t FSARequiredPoolSize(size_t num_of_blocks, size_t block_size)
{
	size_t reminder = block_size & (WORD_SIZE - 1);
	size_t result = 0;
	
	assert(0 != num_of_blocks);
	assert(0 != block_size);
	
	block_size += BYTES_TO_ALIGN_UP(reminder);
	result = num_of_blocks * block_size + sizeof(fsa_t) + POOL_ALIGN_UP;
	
	return result;
}

fsa_t *FSAInit(void *memory_pool, size_t num_of_blocks, size_t block_size)
{
	char *runner = NULL;
	size_t offset_counter = 0;
	
	assert(NULL != memory_pool);
	assert(0 != num_of_blocks);
	assert(0 != block_size);
	
	FSASetUp(&memory_pool, &block_size);
	
	runner = (char *)memory_pool + sizeof(fsa_t);
	
	FreeListSetUp(runner, &offset_counter, block_size, num_of_blocks);
	
	*(size_t *)memory_pool = offset_counter;
	
	return ((fsa_t *)memory_pool);
}

void *FSAAllocate(fsa_t *fsa)
{
	void *free_block = NULL;
	
	assert(NULL != fsa);
	
	if (0 != fsa->free_block)
	{
		free_block = (char *)fsa + fsa->free_block;
		fsa->free_block = *(size_t *)free_block;
	}
	
	return free_block;
}

void FSAFree(fsa_t *fsa, void *block)
{
	assert(NULL != fsa);
	
	*(size_t *)block = fsa->free_block;
	fsa->free_block = (size_t)block - (size_t)fsa;
}

size_t FSACountFree(const fsa_t *fsa)
{
	size_t counter = 0;
	size_t runner = 0;
	
	assert(NULL != fsa);
	
	runner = fsa->free_block;
	
	while (0 != runner)
	{
		++counter;
		runner = *(size_t *)((char *)fsa + runner);
	}
	
	return counter;
}

/*********************************static funcs*********************************/

static void FSASetUp(void **memory_pool, size_t *block_size)
{
	size_t pool_shift = 0;
	
	assert(NULL != memory_pool);
	assert(NULL != block_size);
	assert(NULL != *memory_pool);
	
	BlockSetUp(block_size);
	
	pool_shift = *(size_t *)memory_pool & (WORD_SIZE - 1);
	*(char **)memory_pool += BYTES_TO_ALIGN_UP(pool_shift);
}

static void BlockSetUp(size_t *block_size)
{
	size_t block_shift = 0;
	
	assert(NULL != block_size);
	
	block_shift = *block_size & (WORD_SIZE - 1);
	*block_size += BYTES_TO_ALIGN_UP(block_shift);
}

static void FreeListSetUp(char *runner, size_t *offset_counter, 
							size_t block_size, size_t num_of_blocks)
{
	assert(NULL != runner);
	assert(NULL != offset_counter);
	
	*(size_t *)runner = *offset_counter;
	*offset_counter += sizeof(fsa_t);
	
	while (0 < --num_of_blocks)
	{
		runner += block_size;
		*(size_t *)runner = *offset_counter;
		*offset_counter += block_size;
	}
}


