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
	printf("hello %s\n", msg);
}

static void bye(void *msg)
{
	printf("bye %s\n", msg);
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_task_init(&task_hello, "hello", hello, "world!", &task_hello_stack, sizeof(task_hello_stack));
	xcoro_task_init(&task_bye, "bye", bye, "world!", &task_bye_stack, sizeof(task_bye_stack));
	xcoro_run();
	return 0;
}
