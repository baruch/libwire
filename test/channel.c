#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_stack.h"
#include "xcoro_channel.h"

#include <stdio.h>

static xcoro_t xcoro_main;
static xcoro_task_t task_producer;
static xcoro_task_t task_consumer;
static xcoro_channel_t ch;

static void producer(void *arg)
{
	UNUSED(arg);
	int val = 42;
	xcoro_channel_send(&ch, &val);
	printf("Producer finished\n");
}

static void consumer(void *arg)
{
	UNUSED(arg);
	void *msg;
	xcoro_channel_recv(&ch, &msg);

	int val = *(int *)msg;
	printf("Consumer got %d\n", val);
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_fd_init();
	xcoro_channel_init(&ch);
	xcoro_task_init(&task_producer, "producer", producer, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_task_init(&task_consumer, "consumer", consumer, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_run();
	return 0;
}
