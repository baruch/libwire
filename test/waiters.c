#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_channel.h"

#include <stdio.h>

static wire_thread_t wire_main;
static wire_t task_master;
static wire_t task_io;
static wire_channel_t ch_io;

static void master(void *arg)
{
	UNUSED(arg);
	int val = 0;
	while (val >= 0) {
		wire_channel_t ch_reply;
		wire_channel_init(&ch_reply);

		wire_channel_send(&ch_io, &ch_reply);

		void *val_void;
		wire_channel_recv_block(&ch_reply, &val_void);

		val = *(int*)val_void;
		printf("Got %d\n", val);
	}
}

static void io(void *arg)
{
	UNUSED(arg);
	int val = 1;
	while (1) {
		wire_channel_t *ch_reply;
		wire_channel_recv_block(&ch_io, (void**)&ch_reply);

		wire_fd_wait_msec(1000);

		wire_channel_send(ch_reply, &val);
		val--;
	}
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_channel_init(&ch_io);
	wire_init(&task_master, "mster", master, NULL, WIRE_STACK_ALLOC(4096));
	wire_init(&task_io, "io", io, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
