#include "wire_stack.h"
#include "valgrind_internal.h"
#include "macros.h"
#include "wire.h"

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

#ifdef COVERAGE
void __gcov_flush(void);
#else
static inline void __gcov_flush(void) {}
#endif

void *wire_stack_alloc(unsigned stack_size)
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
		abort();
	}

	base += stack_size + page_size;
	used_size += stack_size + page_size;

	(void)VALGRIND_STACK_REGISTER(ptr, ptr + stack_size);
	return ptr;
}

static const char *sigsegv_code(int si_code)
{
	switch (si_code) {
		case SI_USER: return "kill";
		case SI_KERNEL: return "kernel";
		case SI_TKILL: return "tkill";
		case SEGV_MAPERR: return "address not mapped";
		case SEGV_ACCERR: return "address lacks access permission";
		default: return "Unknown";
	}
}

static void sigsegv_handler(int sig, siginfo_t *si, void *unused)
{
	UNUSED(unused);
	UNUSED(sig);

	wire_t *wire = wire_get_current();

	fprintf(stderr, "Current running wire: %s\n", wire->name);
	fprintf(stderr, "Got SIGSEGV at address: %p reason: %s\n", si->si_addr, sigsegv_code(si->si_code));
	fprintf(stderr, "Current wire: %p stack: %p - %p\n", wire, wire->stack, wire->stack + wire->stack_size);

	if (wire->stack - 4096 < si->si_addr && si->si_addr < wire->stack)
		fprintf(stderr, "Stack overflow happened\n");
	else if (wire->stack + wire->stack_size < si->si_addr && si->si_addr < wire->stack + wire->stack_size + 4096)
		fprintf(stderr, "Stack underflow happened\n");

	__gcov_flush();
	raise(SIGSEGV);
}

void wire_stack_fault_detector_install(void)
{
	struct sigaction sa;
	stack_t alt_stack;

	// Set an alternative stack, the one of the coroutine will not be enough
	const int stack_size = 16*1024;
	alt_stack.ss_sp = wire_stack_alloc(stack_size);
	alt_stack.ss_size = stack_size;
	alt_stack.ss_flags = 0;
	sigaltstack(&alt_stack, NULL);

	// Catch the sigsegv
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO|SA_ONSTACK|SA_RESETHAND;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigsegv_handler;
	if (sigaction(SIGSEGV, &sa, NULL) < 0)
		perror("sigaction");
}
