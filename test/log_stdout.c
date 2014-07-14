#include "wire.h"
#include "wire_log.h"
#include "wire_io.h"
#include "wire_fd.h"
#include "wire_stack.h"

#include <string.h>
#include <stdio.h>

static wire_thread_t wire_main;
static wire_t task_log[16];
static wire_t task_init;

static void do_log(void *arg)
{
	const char *name_arg = arg;
	char name[32];
	strncpy(name, name_arg, sizeof(name));
	name[sizeof(name)-1] = 0;

	wire_log(WLOG_INFO, "Start of wire %s (%p)", name, name);
	wire_yield();
	wire_log(WLOG_NOTICE, "End of wire %s", name);
}

static void do_init(void *arg)
{
	UNUSED(arg);

	unsigned i;
	for (i = 0; i < sizeof(task_log)/sizeof(task_log[0]); i++) {
		char name[32];

		snprintf(name, sizeof(name), "log %u", i);
		wire_init(&task_log[i], name, do_log, name, WIRE_STACK_ALLOC(4096));
		wire_yield(); // Let the wire copy the arg to its stack
	}
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_io_init(1);
	wire_log(WLOG_INFO, "Do not crash on logging before wire_log init");
	wire_log_init_stdout();

	wire_init(&task_init, "init", do_init, NULL, WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
