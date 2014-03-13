// This must be included only from the wire.h

#include "list.h"
#include "coro.h"

struct wire {
	coro_context ctx;
	char name[32];
	void (*entry_point)(void *);
	void *arg;
	void *stack;
	unsigned stack_size;
	struct list_head list;
};

struct wire_thread {
	wire_t sched_wire;
	struct list_head ready_list;
	struct list_head suspend_list;
	wire_t *running_wire;
};
