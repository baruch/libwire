#include "xcoro.h"
#include "xcoro_fd.h"
#include "xcoro_stack.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

static xcoro_t xcoro_main;
static xcoro_task_t task_recurse;

static void do_recurse(int count)
{
	printf("level %d\n", count);
	do_recurse(count+1);
}

static void recurser(void *arg)
{
	UNUSED(arg);
	printf("do recurse\n");
	do_recurse(0);
}

static void sigsegv_handler(int sig, siginfo_t *si, void *unused)
{
	UNUSED(unused);
	UNUSED(sig);

	printf("Got SIGSEGV at address: %p\n", si->si_addr);
	xcoro_task_t *task = xcoro_get_current_task();
	printf("Current task: %p stack: %p - %p\n", task, task->stack, task->stack + task->stack_size);

	if (task->stack - 4096 < si->si_addr && si->si_addr < task->stack)
		printf("Stack overflow happened\n");
	else if (task->stack + task->stack_size < si->si_addr && si->si_addr < task->stack + task->stack_size + 4096)
		printf("Stack underflow happened\n");

	exit(EXIT_FAILURE);
}

static void setup_stack_monitor(void)
{
	struct sigaction sa;
	stack_t alt_stack;

	// Set an alternative stack, the one of the coroutine will not be enough
	alt_stack.ss_sp = xcoro_stack_alloc(SIGSTKSZ);
	alt_stack.ss_size = SIGSTKSZ;
	alt_stack.ss_flags = 0;
	sigaltstack(&alt_stack, NULL);

	// Catch the sigsegv
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO|SA_ONSTACK;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigsegv_handler;
	if (sigaction(SIGSEGV, &sa, NULL) < 0)
		perror("sigaction");
}

int main()
{
	setup_stack_monitor();

	xcoro_init(&xcoro_main);
	xcoro_fd_init();

	xcoro_task_init(&task_recurse, "hello", recurser, NULL, xcoro_stack_alloc(4096), 4096);

	xcoro_run();
	return 0;
}
