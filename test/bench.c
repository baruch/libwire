#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_pool.h"
#include "macros.h"
#include <stdio.h>
#include <time.h>
#include <inttypes.h>

static wire_pool_t pool;
static wire_t task_hello;
static wire_t task_bye;

int steps = 100 * 1000 * 1000;
int stop;

static void action(void *UNUSED(msg))
{
	while (--stop > 0) {
		wire_yield();
	}
}

static void action_create(void *count)
{
	int icount = (intptr_t)count;

	wire_yield();
	if (--icount == 0)
		return;
	wire_pool_alloc(&pool, "cont", action_create, (void*)(long)icount);
}


int main()
{
	struct timespec start;
	struct timespec end;

	stop = steps;

	clock_gettime(CLOCK_MONOTONIC, &start);
	wire_thread_init();
	wire_init(&task_hello, "hello", action, NULL, WIRE_STACK_ALLOC(4096));
	wire_init(&task_bye, "bye", action, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("Doing %d steps took %ld seconds and %ld nanoseconds\n", steps, end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);

	unsigned long long steps_factor = steps * 1000;
	unsigned long long msecs = (end.tv_sec - start.tv_sec) * 1000;
	msecs += (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
	printf("yields per sec = %llu\n", steps_factor / msecs);

	wire_pool_init(&pool, NULL, 128, 4096);

	clock_gettime(CLOCK_MONOTONIC, &start);
	wire_pool_alloc(&pool, "start", action_create, (void*)(long)steps);
	wire_thread_run();
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("Doing %d steps took %ld seconds and %ld nanoseconds\n", steps, end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);

	msecs = (end.tv_sec - start.tv_sec) * 1000;
	msecs += (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
	printf("creates per sec = %llu\n", steps_factor / msecs);


	return 0;
}
