/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 5/1/2023	               */
/* WORKSHEET NAME: Fixed-Size Allocator   */
/* REVIEWER: Yuval Atoun		         */
/*--------------------------------------*/

#ifndef __FSA_H__
#define __FSA_H__

#include <stddef.h> /* size_t */

/*-----------------------------------------------------------------------------
intro: fixed-size blocks allocation, is the use of pools for memory management 
		that allows dynamic memory allocation comparable to malloc.
		
main functionalities: FSARequiredPoolSize(),FSAInit(), FSAAllocate(), FSAFree().

-----------------------------------------------------------------------------*/
typedef struct fsa fsa_t;
/*-----------------------------------------------------------------------------
Description: calculates the required Pool size in bytes for the FSA to work.
	
Parameters: num_of_blocks - number of blocks of memory in the pool.
			block_size - the size of each block.
	
Return Value: amount of bytes required for the pool.
	
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
size_t FSARequiredPoolSize(size_t num_of_blocks, size_t block_size);
/*-----------------------------------------------------------------------------
Description: initialize the FSA.
	
Parameters: memory_pool - the memory space to manage.
			num_of_blocks - number of blocks of memory in the pool.
			block_size - the size of each block.

Return Value: pointer to the FSA.

note: memory_pool size needs to be equal to the return value of 
	 FSARequiredPoolSize(), otherwise the behivior is undefined.
	 if  num_of_blocks or block_size equal 0, the behivior is undefined.

Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
fsa_t *FSAInit(void *memory_pool, size_t num_of_blocks, size_t block_size);
/*-----------------------------------------------------------------------------
Description: allocate a block of memory. 
	
Parameters: fsa - pointer to the FSA.
	
Return Value: pointer to the allocated block of memory, if there are no more
			  free blocks to allocate it returns NULL.
	
Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
void *FSAAllocate(fsa_t *fsa);
/*-----------------------------------------------------------------------------
Description: free a block of memory.
	
Parameters: fsa - pointer to the FSA.
			block - the block of memory to free.
	
Return Value: none.

note: if the block is not allocated the behivior is undefined.

Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
void FSAFree(fsa_t *fsa, void *block);
/*-----------------------------------------------------------------------------
Description: count the amount of free blocks of memory in the FSA. 
	
Parameters: fsa - pointer to the FSA.
	
Return Value: returns the amount of free blocks.
	
Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
size_t FSACountFree(const fsa_t *fsa);

#endif /* __FSA_H__ */

