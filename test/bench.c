#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "macros.h"
#include <stdio.h>
#include <time.h>

static wire_thread_t wire_main;
static wire_t task_hello;
static wire_t task_bye;

int steps = 100 * 1000 * 1000;
int stop;

static void action(void *msg)
{
	UNUSED(msg);
	while (--stop > 0) {
		wire_yield();
	}
}

int main()
{
	struct timespec start;
	struct timespec end;

	stop = steps;

	clock_gettime(CLOCK_MONOTONIC, &start);
	wire_thread_init(&wire_main);
	wire_init(&task_hello, "hello", action, NULL, WIRE_STACK_ALLOC(4096));
	wire_init(&task_bye, "bye", action, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("Doing %d steps took %ld seconds and %ld nanoseconds\n", steps, end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);

	unsigned long long steps_factor = steps * 1000;
	unsigned long long msecs = (end.tv_sec - start.tv_sec) * 1000;
	msecs += (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
	printf("yields per sec = %llu\n", steps_factor / msecs);

	return 0;
}
