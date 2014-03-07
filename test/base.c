#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_stack.h"
#include "macros.h"
#include <stdio.h>

static xcoro_t xcoro_main;
static xcoro_task_t task_hello;
static xcoro_task_t task_bye;

static void do_msg(const char *base, const char *msg)
{
	int i = 0;
	while (i++ < 5) {
		printf("%s %s\n", base, msg);
		xcoro_yield();
	}

	printf("Now slowly in %s\n", base);

	for (i = 0; i < 5; i++) {
		xcoro_fd_wait_msec(1000);
		printf("%s slowly %s\n", base, msg);
	}
}

static void hello(void *msg)
{
	do_msg("hello", msg);
}

static void bye(void *msg)
{
	do_msg("bye", msg);
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_fd_init();
	xcoro_task_init(&task_hello, "hello", hello, "world!", XCORO_STACK_ALLOC(4096));
	xcoro_task_init(&task_bye, "bye", bye, "world!", XCORO_STACK_ALLOC(4096));
	xcoro_run();
	return 0;
}
