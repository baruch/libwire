#include "wire.h"
#include "wire_log.h"
#include "wire_io.h"
#include "wire_fd.h"
#include "wire_stack.h"

static wire_thread_t wire_main;
static wire_t task_log1;
static wire_t task_log2;
static wire_t task_log3;

static void do_log(void *arg)
{
	const char *name = arg;

	wire_log(WLOG_INFO, "Start of wire %s (%p)", name, name);
	wire_yield();
	wire_log(WLOG_NOTICE, "End of wire %s", name);
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_io_init(1);
	wire_log(WLOG_INFO, "Do not crash on logging before wire_log init");
	wire_log_init_stdout();
	wire_init(&task_log1, "log 1", do_log, "log 1", WIRE_STACK_ALLOC(4*1024));
	wire_init(&task_log2, "log 2", do_log, "log 2", WIRE_STACK_ALLOC(4*1024));
	wire_init(&task_log3, "log 3", do_log, "log 3", WIRE_STACK_ALLOC(4*1024));
	wire_thread_run();
	return 0;
}
