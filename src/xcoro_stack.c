#include "xcoro_stack.h"
#include "valgrind_internal.h"
#include "macros.h"
#include "xcoro.h"

#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

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

static void sigsegv_handler(int sig, siginfo_t *si, void *unused)
{
	UNUSED(unused);
	UNUSED(sig);

	xcoro_task_t *task = xcoro_get_current_task();

	printf("Current running task: %s\n", task->name);
	printf("Got SIGSEGV at address: %p\n", si->si_addr);
	printf("Current task: %p stack: %p - %p\n", task, task->stack, task->stack + task->stack_size);

	if (task->stack - 4096 < si->si_addr && si->si_addr < task->stack)
		printf("Stack overflow happened\n");
	else if (task->stack + task->stack_size < si->si_addr && si->si_addr < task->stack + task->stack_size + 4096)
		printf("Stack underflow happened\n");

	exit(EXIT_FAILURE);
}

void xcoro_stack_fault_detector_install(void)
{
	struct sigaction sa;
	stack_t alt_stack;

	// Set an alternative stack, the one of the coroutine will not be enough
	alt_stack.ss_sp = xcoro_stack_alloc(SIGSTKSZ);
	alt_stack.ss_size = SIGSTKSZ;
	alt_stack.ss_flags = 0;
	sigaltstack(&alt_stack, NULL);

	// Catch the sigsegv
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO|SA_ONSTACK;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigsegv_handler;
	if (sigaction(SIGSEGV, &sa, NULL) < 0)
		perror("sigaction");
}
