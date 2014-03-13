#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "macros.h"
#include <stdio.h>

static wire_t wire_main;
static wire_task_t task_hello;
static wire_task_t task_bye;

static void do_msg(const char *base, const char *msg)
{
	int i = 0;
	while (i++ < 5) {
		printf("%s %s\n", base, msg);
		wire_yield();
	}

	printf("Now slowly in %s\n", base);

	for (i = 0; i < 5; i++) {
		wire_fd_wait_msec(1000);
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
	wire_init(&wire_main);
	wire_fd_init();
	wire_task_init(&task_hello, "hello", hello, "world!", WIRE_STACK_ALLOC(4096));
	wire_task_init(&task_bye, "bye", bye, "world!", WIRE_STACK_ALLOC(4096));
	wire_run();
	return 0;
}
