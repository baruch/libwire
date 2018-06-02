#include "wire.h"
#include "wire_stack.h"
#include "wire_defer.h"
#include "macros.h"
#include <stdio.h>

static wire_t task_to_cancel;
static wire_t task_canceller;

static void to_cancel(void *arg)
{
	printf("First print\n");
	DEFER({ printf("Cleanup in cancelled wire\n"); });
	wire_yield();
	printf("Second print (shouldn't happen)\n");
}

static void canceller(void *arg)
{
	printf("Cancelling other wire (%p) before second print\n", arg);
	printf("this wire %p\n",  &task_canceller);
	wire_t* wire = arg;
	wire_cancel(wire);
	printf("Other wire cancelled\n");
	wire_yield();
	printf("Done\n");
}

int main()
{
	wire_thread_init();
	wire_init(&task_to_cancel, "to cnacel", to_cancel, NULL, WIRE_STACK_ALLOC(4096));
	wire_init(&task_canceller, "canceller", canceller, &task_to_cancel, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
