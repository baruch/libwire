// This must be included only from the xcoro.h

#include "list.h"

struct cpu_ctx {
    void *esp;
    void *ebp;
    void *eip;
    void *edi;
    void *esi;
    void *ebx;
    void *r1;
    void *r2;
    void *r3;
    void *r4;
    void *r5;
};

struct xcoro_task {
	struct cpu_ctx ctx;
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
