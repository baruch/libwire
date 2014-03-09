// This must be included only from the xcoro.h

#include "list.h"
#include "coro.h"

struct xcoro_task {
	coro_context ctx;
	char name[32];
	void (*entry_point)(void *);
	void *arg;
	void *stack;
	unsigned stack_size;
	struct list_head list;
};

struct xcoro {
	xcoro_task_t sched_task;
	struct list_head ready_list;
	struct list_head suspend_list;
	xcoro_task_t *running_task;
};
