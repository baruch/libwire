#ifndef WIRE_LIB_TASK_POOL_H
#define WIRE_LIB_TASK_POOL_H

/** @file
 * libwire pool.
 */

#include "wire.h"
#include "wire_channel.h"

/** @defgroup TaskPool Task Pool
 * A task pool is used to allow a large number of tasks to be made available
 * for some activity in a way that is bounded to avoid memory overload. The
 * tasks are allocated such that they are reused as soon as possible so that
 * the hottest one will be used faster to save some cpu cache warming time.
 */
/// @{

struct wire_gpool_entry {
	struct list_head list;
	wire_t task;
	void *stack;
};
typedef struct wire_gpool_entry wire_pool_entry_t;

struct wire_gpool {
	struct wire_gpool_entry *entries;
	struct list_head free_list;
	unsigned size;
	unsigned num_inited;
	unsigned stack_size;
	unsigned block_count;
	wire_channel_t block_ch;
};
typedef struct wire_gpool wire_pool_t;

/** Initialize a task pool. Sets the pool structure with the entries array of size size and will allocate stacks of size stack_size.
 *
 * @param[in] pool The pool to initialize.
 * @param[in] entries An array of all the task pool entries, this reserves the space for the tasks but they need not be initialized in advance or even zeroed. They can be left untouched. The stack pointer in them can either be preallocated or just left untouched and it will be allocate on the first use of the associated task, the stack will be allocated by using wire_stack_alloc() with the pool stack_size.
 * @param[in] size The size of the entries array.
 * @param[in] stack_size The stack size to be used for the tasks. If the stacks were preallocated they must all be the same size and the size needs to be reported correctly here.
 * @return 0 for ok,<br>
 *         -1 when an error occurred and the pool cannot be initialized.
 */
int wire_pool_init(wire_pool_t *pool, wire_pool_entry_t *entries, unsigned size, unsigned stack_size);

/** Allocate a new task from the task pool. Gives the task a name and a
 * function to call and an argument. When the task finished it will
 * automatically be returned to the pool.
 *
 * @param[in] pool The pool to allocate a task from.
 * @param[in] name The name to give the task.
 * @param[in] entry_point The entry point function, it takes a void pointer argument and returns nothing.
 * @param[in] arg The argument to pass to the entry point function when it starts.
 * @return An allocated and initialized task. If there is no entry available in the
 * pool it will return NULL.
 */
wire_t *wire_pool_alloc(wire_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg);

/** Similar to wire_pool_alloc() but this one will block until a wire is available in the pool.
 *
 * @param[in] pool The pool to allocate a task from.
 * @param[in] name The name to give the task.
 * @param[in] entry_point The entry point function, it takes a void pointer argument and returns nothing.
 * @param[in] arg The argument to pass to the entry point function when it starts.
 * @return An allocated and initialized task. Will rescheduled the current wire until a wire is available in the pool.
 */
wire_t *wire_pool_alloc_block(wire_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg);

/// @}

#endif
