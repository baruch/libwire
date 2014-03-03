#include "xcoro_stack.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>

static void *base;
static const unsigned alloc_size = 10*1024*1024;
static unsigned used_size;

void *xcoro_stack_alloc(unsigned stack_size)
{
	unsigned stack_size_pages = (stack_size + 4095) / 4096;
	stack_size = stack_size_pages * 4096;
	unsigned full_size = stack_size + 4096;

	if (base == NULL || used_size + full_size > alloc_size) {
		void *ptr = mmap(0, alloc_size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		if (ptr == MAP_FAILED) {
			perror("Failed to allocate space for stacks");
			return NULL;
		}

		base = ptr + 4096;
		used_size = 4096;
	}

	void *ptr = base;
	int ret = mprotect(ptr, stack_size, PROT_READ|PROT_WRITE);
	if (ret < 0) {
		perror("failed to mprotect stack for read/write");
		return NULL;
	}

	base += stack_size + 4096;
	used_size += stack_size + 4096;

	return ptr;
}
