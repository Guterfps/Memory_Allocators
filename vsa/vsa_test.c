/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 9/1/2023	               */
/* WORKSHEET NAME: Variable-Size Allocator*/
/* REVIEWER: Dotan				         */
/*--------------------------------------*/

#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* malloc(), free() */

#include "vsa.h"	/* function declarations and typedefs */

#define POOL_SIZE 123
#define WORD_SIZE (sizeof(size_t))

void Test1(void);
void Test2(void);

int main(void)
{
	Test1();
	Test2();
	
	return 0;
}

void Test1(void)
{
	void *pool = malloc(POOL_SIZE);
	vsa_t *vsa = VSAInit(pool, POOL_SIZE);
	void *alloc_test1 = NULL;
	void *alloc_test2 = NULL;
	void *alloc_test3 = NULL;
	void *alloc_test4 = NULL;
	void *alloc_test5 = NULL;
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	((POOL_SIZE - 27) == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("allocs and frees: \n");
	alloc_test1 = VSAAllocate(vsa, 3);
	alloc_test2 = VSAAllocate(vsa, 21);
	alloc_test3 = VSAAllocate(vsa, 210);
	alloc_test1 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	alloc_test2 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	(NULL == alloc_test3) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	*(char *)alloc_test1 = 'a';
	*(long double *)alloc_test2 = 123123.123;
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(32 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");

	VSAFree(alloc_test1);
	alloc_test4 = VSAAllocate(vsa, 1);
	alloc_test4 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	*(char *)alloc_test4 = 'a';
	alloc_test5 = VSAAllocate(vsa, 32);
	alloc_test5 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	*(long double *)alloc_test5 = -123123123.123;
	alloc_test3 = VSAAllocate(vsa, 1);
	(NULL == alloc_test3) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(0 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	VSAFree(alloc_test2);
	alloc_test2 = VSAAllocate(vsa, 10);
	alloc_test2 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	*(double *)alloc_test2 = -123.123;
	alloc_test3 = VSAAllocate(vsa, 1);
	(NULL == alloc_test3) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("alignment test:	");
	!((size_t)alloc_test2 % WORD_SIZE) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("alignment test:	");
	!((size_t)alloc_test4 % WORD_SIZE) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	VSAFree(alloc_test2);
	VSAFree(alloc_test5);	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(72 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	alloc_test1 = VSAAllocate(vsa, 23);

	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(32 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	alloc_test5 = VSAAllocate(vsa, 7);
	alloc_test5 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(8 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	VSAFree(alloc_test1);
	VSAFree(alloc_test5);
	VSAFree(alloc_test4);
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	((POOL_SIZE - 27) == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	
	free(pool);
	pool = NULL;
}

void Test2(void)
{
	void *pool = malloc(POOL_SIZE);
	vsa_t *vsa = VSAInit(pool, POOL_SIZE);
	void *alloc_test1 = NULL;
	void *alloc_test2 = NULL;
	
	printf("Test2:	\n");
	printf("allocs and frees: \n");
	alloc_test1 = VSAAllocate(vsa, 48);
	alloc_test2 = VSAAllocate(vsa, 5);
	alloc_test1 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	alloc_test2 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	*(char *)alloc_test2 = 'a';
	*((char *)alloc_test2 + 1) = 'h';
	*((char *)alloc_test2 + 2) = 'i';
	*((char *)alloc_test2 + 3) = '\0';
	printf("%s\n", (char *)alloc_test2);
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(8 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	!VSARealloc(vsa, alloc_test2, 10) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	VSAFree(alloc_test1);
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(48 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	alloc_test2 = VSARealloc(vsa, alloc_test2, 10);
	alloc_test2 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("%s\n", (char *)alloc_test2);
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(64 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	alloc_test2 = VSARealloc(vsa, alloc_test2, 20);
	alloc_test2 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("%s\n", (char *)alloc_test2);
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(56 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	alloc_test2 = VSARealloc(vsa, alloc_test2, 40);
	alloc_test2 ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("%s\n", (char *)alloc_test2);
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(40 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	alloc_test1 = VSAAllocate(vsa, 40);
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(0 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	!VSARealloc(vsa, alloc_test2, 40) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	VSAFree(alloc_test1);
	VSAFree(alloc_test2);
	
	printf("MaxFreeBlock test:	");
	printf("%ld, ", VSAGetMaxFreeBlockSize(vsa));
	(96 == VSAGetMaxFreeBlockSize(vsa)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	free(pool);
	pool = NULL;
}


