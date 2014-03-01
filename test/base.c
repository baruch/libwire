#include "xcoro.h"
#include <stdio.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

xcoro_t xcoro_main;
xcoro_task_t task_hello;
char task_hello_stack[4096];

static void hello(void *msg)
{
	printf("hello %s\n", msg);
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_task_init(&task_hello, "hello", hello, "world!", &task_hello_stack, sizeof(task_hello_stack));
	xcoro_run();
	return 0;
}
