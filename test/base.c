#include "xcoro.h"
#include "macros.h"
#include <stdio.h>

xcoro_t xcoro_main;
xcoro_task_t task_hello;
char task_hello_stack[4096];
xcoro_task_t task_bye;
char task_bye_stack[4096];

static void hello(void *msg)
{
	int i = 0;
	while (i++ < 5) {
		printf("hello %s\n", msg);
		xcoro_yield();
	}
}

static void bye(void *msg)
{
	int i = 0;
	while (i++ < 6) {
		printf("bye %s\n", msg);
		xcoro_yield();
	}
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_task_init(&task_hello, "hello", hello, "world!", &task_hello_stack, sizeof(task_hello_stack));
	xcoro_task_init(&task_bye, "bye", bye, "world!", &task_bye_stack, sizeof(task_bye_stack));
	xcoro_run();
	return 0;
}
