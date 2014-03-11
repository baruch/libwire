#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_stack.h"
#include "macros.h"
#include <stdio.h>
#include <time.h>

static xcoro_t xcoro_main;
static xcoro_task_t task_hello;
static xcoro_task_t task_bye;

int steps = 100 * 1000 * 1000;
int stop;

static void action(void *msg)
{
	UNUSED(msg);
	while (--stop > 0) {
		xcoro_yield();
	}
}

int main()
{
	struct timespec start;
	struct timespec end;

	stop = steps;

	clock_gettime(CLOCK_MONOTONIC, &start);
	xcoro_init(&xcoro_main);
	xcoro_task_init(&task_hello, "hello", action, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_task_init(&task_bye, "bye", action, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_run();
	clock_gettime(CLOCK_MONOTONIC, &end);

	printf("Doing %d steps took %ld seconds and %ld nanoseconds\n", steps, end.tv_sec - start.tv_sec, end.tv_nsec - start.tv_nsec);

	unsigned long long steps_factor = steps * 1000;
	unsigned long long msecs = (end.tv_sec - start.tv_sec) * 1000;
	msecs += (end.tv_nsec - start.tv_nsec) / (1000 * 1000);
	printf("yields per sec = %llu\n", steps_factor / msecs);

	return 0;
}
