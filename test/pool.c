#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_pool.h"

#include <stdio.h>

static wire_thread_t wire_main;
static wire_pool_t wire_pool;
static wire_t wire_init_;

static const int num_wires = 8;

static void func(void *arg)
{
	int num = (long long int)arg;

	printf("wire started %d\n", num);
	wire_fd_wait_msec(num * 50);
	printf("wire done %d\n", num);
}

static void init_func(void *arg)
{
	UNUSED(arg);
	int i;
	for (i = 0; i < num_wires; i++) {
		char name[16];
		snprintf(name, sizeof(name), "pool %d", i);
		wire_pool_alloc_block(&wire_pool, name, func, (void*)(long long int)i);
	}

	// Wait for all of the pool entries to get to the free block
	wire_fd_wait_msec(500);

	// Now we do the same but first allocate from the free block
	for (i = 0; i < num_wires; i++) {
		char name[16];
		snprintf(name, sizeof(name), "pool %d", i);
		wire_pool_alloc_block(&wire_pool, name, func, (void*)(long long int)i);
	}
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();

	// Allocate a smaller pool so we also get to the blocked part
	wire_pool_init(&wire_pool, NULL, num_wires/2, 4096);
	wire_init(&wire_init_, "init", init_func, NULL, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
	return 0;
}
