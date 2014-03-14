#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_channel.h"

#include <stdio.h>

static wire_thread_t wire_main;
static wire_t task_producer;
static wire_t task_consumer;
static wire_channel_t ch;

static void producer(void *arg)
{
	UNUSED(arg);
	int val = 42;
	wire_channel_send(&ch, &val);
	printf("Producer finished\n");
}

static void consumer(void *arg)
{
	UNUSED(arg);
	void *msg;
	wire_channel_recv_block(&ch, &msg);

	int val = *(int *)msg;
	printf("Consumer got %d\n", val);
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_channel_init(&ch);
	wire_init(&task_producer, "producer", producer, NULL, WIRE_STACK_ALLOC(4096));
	wire_init(&task_consumer, "consumer", consumer, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
