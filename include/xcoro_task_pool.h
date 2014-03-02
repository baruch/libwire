#ifndef XCORO_LIB_TASK_POOL_H
#define XCORO_LIB_TASK_POOL_H

#include "xcoro.h"

struct xcoro_task_pool_entry {
	struct list_head list;
	xcoro_task_t task;
	void *stack;
};
typedef struct xcoro_task_pool_entry xcoro_task_pool_entry_t;

struct xcoro_task_pool {
	struct xcoro_task_pool_entry *entries;
	struct list_head free_list;
	struct list_head active_list;
	unsigned size;
	unsigned num_inited;
	unsigned stack_size;
};
typedef struct xcoro_task_pool xcoro_task_pool_t;

int xcoro_task_pool_init(xcoro_task_pool_t *pool, xcoro_task_pool_entry_t *entries, unsigned size, unsigned stack_size);
xcoro_task_t *xcoro_task_pool_alloc(xcoro_task_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg);

#endif
