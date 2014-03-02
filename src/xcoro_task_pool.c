#include "xcoro_task_pool.h"
#include "list.h"
#include <stdlib.h>

#include <sys/mman.h>

struct wrapper_args {
	void (*entry_point)(void *);
	void *arg;
	xcoro_task_t *source_task;
	xcoro_task_pool_entry_t *entry;
	xcoro_task_pool_t *pool;
};

/* It should be possible to save address space if a large chunk of memory was
 * mmaped and then between each two stacks there was only one guard page. In
 * the current scheme there is a guard page before and after every stack.
 */
static void *stack_alloc(unsigned stack_size)
{
	unsigned stack_size_pages = (stack_size + 4095) / 4096;
	stack_size = stack_size_pages * 4096;
	unsigned full_size = stack_size + 4096*2;
	void *ptr = mmap(0, full_size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED)
		return NULL;

	int ret = mprotect(ptr + 4096, stack_size, PROT_READ|PROT_WRITE);
	if (ret < 0) {
		perror("failed to mprotect stack for read/write");
		munmap(ptr, full_size);
		return NULL;
	}

	return ptr+4096;
}

int xcoro_task_pool_init(xcoro_task_pool_t *pool, xcoro_task_pool_entry_t *entries, unsigned size, unsigned stack_size)
{
	if (entries == NULL)
		entries = calloc(size, sizeof(xcoro_task_pool_entry_t));

	pool->size = size;
	pool->num_inited = 0;
	pool->stack_size = stack_size;
	pool->entries = entries;
	list_head_init(&pool->free_list);
	list_head_init(&pool->active_list);
}

static void wrapper_entry_point(void *arg)
{
	struct wrapper_args *args = arg;
	void (*entry_point)(void*) = args->entry_point;
	arg = args->arg;
	xcoro_task_pool_entry_t *entry = args->entry;
	xcoro_task_pool_t *pool = args->pool;

	xcoro_resume(args->source_task);

	entry_point(arg);

	list_move_head(&entry->list, &pool->free_list);
}

xcoro_task_t *xcoro_task_pool_alloc(xcoro_task_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg)
{
	xcoro_task_pool_entry_t *entry;

	if (!list_empty(&pool->free_list)) {
		entry = list_entry(list_head(&pool->free_list), xcoro_task_pool_entry_t, list);
		list_del(&entry->list);
	} else if (pool->num_inited < pool->size) {
		// Not everything was initialized yet, initialize another one
		unsigned index = pool->num_inited++;
		entry = &(pool->entries[index]);
		if (!entry->stack)
			entry->stack = stack_alloc(pool->stack_size);
	} else {
		// No place in pool
		return NULL;
	}

	list_add_head(&entry->list, &pool->active_list);
	struct wrapper_args args = {
		.entry_point = entry_point,
		.arg = arg,
		.source_task = xcoro_get_current_task(),
		.entry = entry,
		.pool = pool,
	};
	xcoro_task_init(&entry->task, name, wrapper_entry_point, &args, entry->stack, pool->stack_size);
	xcoro_suspend();
	return &entry->task;
}
