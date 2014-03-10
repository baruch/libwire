#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_stack.h"
#include "macros.h"
#include <stdio.h>

static xcoro_t xcoro_main;
static xcoro_task_t task_recurse;

static void do_recurse(int count)
{
	printf("level %d\n", count);
	do_recurse(count+1);
}

static void recurser(void *arg)
{
	UNUSED(arg);
	printf("do recurse\n");
	do_recurse(0);
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_fd_init();
	xcoro_task_init(&task_recurse, "hello", recurser, NULL, xcoro_stack_alloc(4096), 4096);
	xcoro_run();
	return 0;
}
