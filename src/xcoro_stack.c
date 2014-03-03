#include "xcoro_stack.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

void *xcoro_stack_alloc(unsigned stack_size)
{
	unsigned stack_size_pages = (stack_size + 4095) / 4096;
	stack_size = stack_size_pages * 4096;
	unsigned full_size = stack_size + 4096*2;
	void *ptr = mmap(0, full_size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED)
		return NULL;

	int ret = mprotect(ptr + 4096, stack_size, PROT_READ|PROT_WRITE);
	if (ret < 0) {
		perror("failed to mprotect stack for read/write");
		munmap(ptr, full_size);
		return NULL;
	}

	return ptr+4096;
}
