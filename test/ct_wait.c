#include "wire.h"
#include "wire_stack.h"
#include "wire_wait.h"

#include "../test/ct/ct.h"

static wire_thread_t wire_thread_main;
static wire_t wire1;
static wire_t wire2;
static int stage;

static void test_releaser(void *arg)
{
	wire_wait_t *wait = arg;

	stage = 1;
	wire_wait_resume(wait);
	stage = 2;
}

static void test_waiter(void *arg)
{
	assert(arg == NULL);

	stage = 0;

	wire_wait_t wait;
	wire_wait_init(&wait);

	wire_init(&wire2, "releaser", test_releaser, &wait, WIRE_STACK_ALLOC(4096));

	wire_wait_single(&wait);
	assert(stage == 2);
}

void cttest_wait_one(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire1, "waiter", test_waiter, NULL, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}

///////////////////////////////////////////////////////////////////////////////////////

static void test_releaser_late(void *arg)
{
	wire_wait_t *wait = arg;

	stage = 1;
	wire_yield();
	stage = 3;
	wire_wait_resume(wait);
	stage = 2;
}

static void test_waiter_late(void *arg)
{
	assert(arg == NULL);

	stage = 0;

	wire_wait_t wait;
	wire_wait_init(&wait);

	wire_init(&wire2, "releaser", test_releaser_late, &wait, WIRE_STACK_ALLOC(4096));

	wire_wait_single(&wait);
	assert(stage == 2);
}

void cttest_wait_one_late(void)
{
	wire_thread_init(&wire_thread_main);

	wire_init(&wire1, "waiter", test_waiter_late, NULL, WIRE_STACK_ALLOC(4096));

	wire_thread_run();
}
