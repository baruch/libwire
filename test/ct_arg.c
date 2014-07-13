#include "wire.h"
#include "wire_stack.h"

#include "../test/ct/ct.h"

static wire_thread_t wire_thread_main;
static wire_t wire1;
static wire_t wire2;

static void test_arg_null(void *arg)
{
	assertf(arg == NULL, "arg is not null");
}

void cttest_arg_null(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire1, "wire", test_arg_null, NULL, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}


static void test_arg_wire(void *arg)
{
	assertf(arg == &wire2, "arg is incorrect!");
}

void cttest_arg_wire(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire2, "wire", test_arg_wire, &wire2, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}

void cttest_arg_both(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire1, "wire", test_arg_null, NULL, WIRE_STACK_ALLOC(4096));
	wire_init(&wire2, "wire", test_arg_wire, &wire2, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}
