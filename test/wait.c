#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_wait.h"

#include <stdio.h>

static wire_thread_t wire_main;
static wire_t task_producer;
static wire_t task_consumer;

static void consumer(void *arg)
{
	wire_wait_t *wait = arg;

	printf("Consumer starting\n");

	printf("Wake up producer\n");
	wire_wait_resume(wait);

	wire_fd_wait_msec(50);
	printf("Wake up producer with no impact\n");
	wire_wait_resume(wait);

	wire_fd_wait_msec(250);
	printf("Wake up producer last time\n");
	wire_wait_resume(wait);

	printf("Consumer finished\n");
}

static void producer(void *arg)
{
	UNUSED(arg);
	wire_wait_t wait;
	wire_wait_list_t wait_list;

	wire_wait_init(&wait);
	wire_wait_list_init(&wait_list);
	wire_wait_chain(&wait_list, &wait);

	printf("Producer starting\n");
	wire_init(&task_consumer, "consumer", consumer, &wait, WIRE_STACK_ALLOC(4096));

	wire_list_wait(&wait_list);
	printf("We've been woken up\n");

	printf("Now wait out the first wait\n");
	wire_fd_wait_msec(100);

	printf("Now reset the wait and wait for it again\n");
	wire_wait_reset(&wait);
	wire_list_wait(&wait_list);

	wire_wait_unchain(&wait);
	printf("Producer finished\n");
}


int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_init(&task_producer, "producer", producer, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
