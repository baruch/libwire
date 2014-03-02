#include "xcoro.h"
#include "xcoro_fd.h"
#include "macros.h"
#include <stdio.h>

xcoro_t xcoro_main;
xcoro_task_t task_hello;
char task_hello_stack[4096];
xcoro_task_t task_bye;
char task_bye_stack[4096];

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
	xcoro_task_init(&task_hello, "hello", hello, "world!", &task_hello_stack, sizeof(task_hello_stack));
	xcoro_task_init(&task_bye, "bye", bye, "world!", &task_bye_stack, sizeof(task_bye_stack));
	xcoro_run();
	return 0;
}
