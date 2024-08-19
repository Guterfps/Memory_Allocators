/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 9/1/2023	               */
/* WORKSHEET NAME: Variable-Size Allocator*/
/* REVIEWER: Dotan				         */
/*--------------------------------------*/

#ifndef __VSA_H__
#define __VSA_H__

#include <stddef.h> /* size_t */

/*-----------------------------------------------------------------------------
Description: memory managment tool, used for allocating blocks of memory 
			 of any size. 

main functionalities: VSAInit(), VSAAllocate(), VSAFree().
-----------------------------------------------------------------------------*/

/* vsa handler */
typedef struct vsa vsa_t;

/*-----------------------------------------------------------------------------
Description: initializing the VSA, and returning a pointer to it.
	
Parameters: memory_pool - pointer to the memory to be managed.
			pool_size - the size of the memory in bytes, 
						minimum size must be 64 bytes or more.
	
Return Value: pointer to the new VSA.

note: some of the memory will be reserved for managment, hance not all of the
	  memory will be useable.

Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
vsa_t *VSAInit(void *memory_pool, size_t pool_size);
/*-----------------------------------------------------------------------------
Description: allocating a block of memory.
	
Parameters: vsa - a pointer to a VSA.
			block_size - the size of memory to be allocated in bytes,
						 if block_size equal to 0 behivior is undefined.
	
Return Value: returns pointer to the allocated block of memory, if no block 
			  of the requasted block_size is available it returns NULL.

note: you may want to use VSAGetMaxFreeBlockSize() before allocating.

Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
void *VSAAllocate(vsa_t *vsa, size_t block_size);
/*-----------------------------------------------------------------------------
Description: frees a block of memory.
	
Parameters: block - pointer to the block to be freed.
	
Return Value: none.

note: if the block is not part of the memory pool the behaviour undefined.

Complexity:
	time: o(1), space: o(1) 
-----------------------------------------------------------------------------*/
void VSAFree(void *block);
/*-----------------------------------------------------------------------------
Description: finds the largest free block.
	
Parameters: vsa - a pointer to a VSA.
	
Return Value: returns the size of the largest free block(Continuously), 
			  or 0 if no block is free.
	
Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
size_t VSAGetMaxFreeBlockSize(vsa_t *vsa);

/*-----------------------------------------------------------------------------
Description: finds a new block of size new_size.
	
Parameters: vsa - a pointer to a VSA.
			block - pointer to the block to resize.
			new_size - the new size of the block.

Return Value: pointer to the resized block or NULL if there is not enough 
			  memory for resize. 

note: if block is not from the vsa the behivior is undefined.
self note: not tested enough probebly have more bugs.

Complexity:
	time: o(n), space: o(1) 
-----------------------------------------------------------------------------*/
void *VSARealloc(vsa_t *vsa, void *block, size_t new_size);

#endif /* __VSA_H__ */

