#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_stack.h"
#include "xcoro_channel.h"

#include <stdio.h>

static xcoro_t xcoro_main;
static xcoro_task_t task_master;
static xcoro_task_t task_io;
static xcoro_channel_t ch_io;

static void master(void *arg)
{
	UNUSED(arg);
	int val = 0;
	while (val >= 0) {
		xcoro_channel_t ch_reply;
		xcoro_channel_init(&ch_reply);

		xcoro_channel_send(&ch_io, &ch_reply);

		void *val_void;
		xcoro_channel_recv(&ch_reply, &val_void);

		val = *(int*)val_void;
		printf("Got %d\n", val);
	}
}

static void io(void *arg)
{
	UNUSED(arg);
	int val = 1;
	while (1) {
		xcoro_channel_t *ch_reply;
		xcoro_channel_recv(&ch_io, (void**)&ch_reply);

		xcoro_fd_wait_msec(1000);

		xcoro_channel_send(ch_reply, &val);
		val--;
	}
}

int main()
{
	xcoro_init(&xcoro_main);
	xcoro_fd_init();
	xcoro_channel_init(&ch_io);
	xcoro_task_init(&task_master, "mster", master, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_task_init(&task_io, "io", io, NULL, XCORO_STACK_ALLOC(4096));
	xcoro_run();
	return 0;
}
