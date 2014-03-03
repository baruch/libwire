#include "xcoro_stack.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#ifdef USE_VALGRIND
#include "valgrind/valgrind.h"
#else
#define VALGRIND_STACK_REGISTER(start, end)
#endif

static unsigned page_size;
static void *base;
static const unsigned alloc_size = 10*1024*1024;
static unsigned used_size;

void *xcoro_stack_alloc(unsigned stack_size)
{
	if (!page_size)
		page_size = sysconf(_SC_PAGE_SIZE);

	unsigned stack_size_pages = (stack_size + page_size - 1) / page_size;
	stack_size = stack_size_pages * page_size;
	unsigned full_size = stack_size + page_size;

	if (base == NULL || used_size + full_size > alloc_size) {
		void *ptr = mmap(0, alloc_size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
		if (ptr == MAP_FAILED) {
			perror("Failed to allocate space for stacks");
			return NULL;
		}

		base = ptr + page_size;
		used_size = page_size;
	}

	void *ptr = base;
	int ret = mprotect(ptr, stack_size, PROT_READ|PROT_WRITE);
	if (ret < 0) {
		perror("failed to mprotect stack for read/write");
		return NULL;
	}

	base += stack_size + page_size;
	used_size += stack_size + page_size;

	VALGRIND_STACK_REGISTER(ptr, ptr + stack_size);
	return ptr;
}
