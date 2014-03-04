#ifndef XCORO_LIB_TASK_POOL_H
#define XCORO_LIB_TASK_POOL_H

/** @file
 * XCoro task pool.
 */

#include "xcoro.h"

/** @defgroup TaskPool Task Pool
 * A task pool is used to allow a large number of tasks to be made available
 * for some activity in a way that is bounded to avoid memory overload. The
 * tasks are allocated such that they are reused as soon as possible so that
 * the hottest one will be used faster to save some cpu cache warming time.
 */
/// @{

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

/** Initialize a task pool. Sets the pool structure with the entries array of size size and will allocate stacks of size stack_size. */
int xcoro_task_pool_init(xcoro_task_pool_t *pool, xcoro_task_pool_entry_t *entries, unsigned size, unsigned stack_size);

/** Allocate a new task from the task pool. Gives the task a name and a
 * function to call and an argument. When the task finished it will
 * automatically be returned to the pool. If there is no entry available in the
 * pool it will return NULL. */
xcoro_task_t *xcoro_task_pool_alloc(xcoro_task_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg);

/// @}

#endif
