// This must be included only from the wire.h

#include "list.h"
#include "coro.h"

struct wire {
	coro_context ctx;
	struct list_head list;
	char name[32];
	void (*entry_point)(void *);
	void *arg;
#ifdef USE_VALGRIND
	void *stack;
	unsigned stack_size;
#endif
};

struct wire_thread {
	wire_t *running_wire;
	struct list_head ready_list;
	struct list_head suspend_list;
	wire_t sched_wire;
};
