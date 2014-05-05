
#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_lock.h"
#include "wire_pool.h"

#include <stdio.h>

static wire_thread_t wire_main;
static wire_pool_t wire_pool;
static wire_lock_t lock;

static void lock_func(void *arg)
{
	int num = (long long int)arg;

	wire_lock_take(&lock);

	printf("Lock taken from %d\n", num);
	wire_fd_wait_msec(num * 500);

	printf("Releasing lock from %d\n", num);
	wire_lock_release(&lock);
	printf("Lock released from %d\n", num);
}

static void lock_close_func(void *arg)
{
	UNUSED(arg);

	wire_fd_wait_msec(5*1000);
	printf("Trying to close the lock now\n");
	wire_lock_wait_clear(&lock);
	printf("Lock is clear\n");
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();

	const int num_wires = 8;
	wire_pool_init(&wire_pool, NULL, num_wires+1, 4096);

	wire_lock_init(&lock);

	wire_pool_alloc(&wire_pool, "lock closer", lock_close_func, NULL);

	int i;
	for (i = 0; i < num_wires; i++) {
		char name[16];
		snprintf(name, sizeof(name), "lock %d", i);
		wire_pool_alloc(&wire_pool, name, lock_func, (void*)(long long int)i);
	}

	wire_thread_run();
	return 0;
}
