#include "wire.h"
#include "wire_stack.h"
#include "wire_wait.h"

#include "../test/ct/ct.h"

static wire_thread_t wire_thread_main;
static wire_t wire1;
static wire_t wire2;
static wire_t wire3;
static wire_wait_t wait1;
static wire_wait_t wait2;

static void test_releaser1_a(void *arg)
{
	wire_wait_t *wait = arg;

	wire_wait_resume(wait);
}

static void test_releaser2_a(void *arg)
{
	wire_wait_t *wait = arg;

	wire_yield();
	wire_wait_resume(wait);
}

static void test_waiter_a(void *arg)
{
	assert(arg == NULL);

	wire_wait_init(&wait1);
	wire_wait_init(&wait2);

	wire_init(&wire2, "releaser1", test_releaser1_a, &wait1, WIRE_STACK_ALLOC(4096));
	wire_init(&wire3, "releaser2", test_releaser2_a, &wait2, WIRE_STACK_ALLOC(4096));

	int ret = wire_wait_two(&wait1, &wait2);
	assert(ret == 1);
}

void cttest_wait2_a(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire1, "waiter", test_waiter_a, NULL, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}

///////////////////////////////////////////////////////////////////////////////////////

static void test_releaser1_b(void *arg)
{
	wire_wait_t *wait = arg;

	wire_yield();
	wire_yield();
	wire_wait_resume(wait);
}

static void test_releaser2_b(void *arg)
{
	wire_wait_t *wait = arg;

	wire_wait_resume(wait);
}

static void test_waiter_b(void *arg)
{
	assert(arg == NULL);

	wire_wait_init(&wait1);
	wire_wait_init(&wait2);

	wire_init(&wire2, "releaser1", test_releaser1_b, &wait1, WIRE_STACK_ALLOC(4096));
	wire_init(&wire3, "releaser2", test_releaser2_b, &wait2, WIRE_STACK_ALLOC(4096));

	int ret = wire_wait_two(&wait1, &wait2);
	assert(ret == 2);
}

void cttest_wait2_b(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire1, "waiter", test_waiter_b, NULL, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}
