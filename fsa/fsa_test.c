/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 5/1/2023	               */
/* WORKSHEET NAME: Fixed-Size Allocator   */
/* REVIEWER: Yuval Atoun		         */
/*--------------------------------------*/

#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* malloc() */

#include "fsa.h"	/* function declerations and typedefs */


void Test1();

int main(void)
{
	Test1();
	
	return 0;
}

void Test1()
{
	fsa_t *fsa = NULL;
	size_t pool_size = FSARequiredPoolSize(5, 100);
	void *pool = malloc(pool_size);
	void *test_alloc[5] = {NULL};
	long test_val[5] = {123123123, 1, 2, 3 ,-123};
	size_t i = 0;
	int status = 1;
	
	fsa = FSAInit(pool, 5, 100);
	
	printf("FSARequiredPoolSize test:	");
	((104 * 5 + 15) == FSARequiredPoolSize(5, 100)) ? 
	printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("free blocks test:	");
	(5 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("alloc test:	");
	
	for (i = 0; (i < 5) && status; ++i)
	{
		test_alloc[i] = FSAAllocate(fsa);
		status = (NULL != test_alloc[i]);
		*(long *)test_alloc[i] = test_val[i];
	}
	
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("free blocks test:	");
	(0 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("alloc when full test:	");
	(NULL == FSAAllocate(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	for (i = 5; (i > 0) && status; --i)
	{
		FSAFree(fsa, test_alloc[i - 1]);
		status = (test_val[i] != *(long *)test_alloc[i - 1]);
	}
	
	printf("free test:	");
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("free blocks test:	");
	(5 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("alloc after free test:	");
	
	for (i = 0; (i < 5) && status; ++i)
	{
		test_alloc[i] = FSAAllocate(fsa);
		status = (NULL != test_alloc[i]);
		*(long *)test_alloc[i] = test_val[i];
	}
	
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("free blocks test:	");
	(0 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	FSAFree(fsa, test_alloc[2]);
	FSAFree(fsa, test_alloc[4]);
	FSAFree(fsa, test_alloc[0]);
	
	printf("free blocks test:	");
	(3 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	test_alloc[0] = FSAAllocate(fsa);
	status *= (NULL != test_alloc[0]);
	test_alloc[2] = FSAAllocate(fsa);
	status *= (NULL != test_alloc[2]);
	printf("free blocks test:	");
	(1 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	test_alloc[4] = FSAAllocate(fsa);
	status *= (NULL != test_alloc[4]);
	printf("free & alloc not in order:	");
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	printf("alloc when full test:	");
	(NULL == FSAAllocate(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	for (i = 0; (i < 5) && status; ++i)
	{
		FSAFree(fsa, test_alloc[i]);
		status = (test_val[i] != *(long *)test_alloc[i]);
	}
	
	printf("free test:	");
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("free blocks test:	");
	(5 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("alloc after free test:	");
	
	for (i = 0; (i < 5) && status; ++i)
	{
		test_alloc[i] = FSAAllocate(fsa);
		status = (NULL != test_alloc[i]);
		*(long *)test_alloc[i] = test_val[i];
	}
	
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	for (i = 0; (i < 5) && status; ++i)
	{
		FSAFree(fsa, test_alloc[i]);
		status = (test_val[i] != *(long *)test_alloc[i]);
	}
	
	printf("free test:	");
	(status) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	printf("free blocks test:	");
	(5 == FSACountFree(fsa)) ? printf("SUCCESS ✅️\n") : printf("FAIL ❌️\n");
	
	free(pool);
	pool = NULL;
}

