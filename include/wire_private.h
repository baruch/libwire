// This must be included only from the wire.h

#include "list.h"

#if USE_LIBCORO
#include "coro.h"
#else
typedef struct coro_context {
	void** sp;
} coro_context;
#endif

struct wire_defer_list {
	struct wire_defer_list* next;
	void (*cleanup)(struct wire_defer_list*);
};

struct wire {
	coro_context ctx; // This must be first in the wire
	struct list_head list;
	volatile int cancelled;
	struct wire_defer_list* cleanup_head;
	char name[32];
#if USE_LIBCORO
	void (*entry_point)(void*);
	void* arg;
#endif
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
