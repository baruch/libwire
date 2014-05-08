#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "macros.h"
#include <stdio.h>

static wire_thread_t wire_main;
static wire_t task_recurse;

static void do_recurse(int count)
{
	if (count > 100000)
		return;
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
	wire_stack_fault_detector_install();
	wire_thread_init(&wire_main);
	wire_fd_init();

	wire_init(&task_recurse, "hello", recurser, NULL, wire_stack_alloc(4096), 4096);

	wire_thread_run();
	return 0;
}
