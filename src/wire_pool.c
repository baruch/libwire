#include "wire_pool.h"
#include "wire_stack.h"
#include "list.h"
#include "valgrind_internal.h"

#include <stdlib.h>
#include <stdio.h>

struct wrapper_args {
	void (*entry_point)(void *);
	void *arg;
	wire_pool_entry_t *entry;
	wire_pool_t *pool;
};

int wire_pool_init(wire_pool_t *pool, wire_pool_entry_t *entries, unsigned size, unsigned stack_size)
{
	if (entries == NULL)
		entries = calloc(size, sizeof(wire_pool_entry_t));

	pool->size = size;
	pool->num_inited = 0;
	pool->stack_size = stack_size;
	pool->entries = entries;
	list_head_init(&pool->free_list);
	pool->block_count = 0;
	wire_channel_init(&pool->block_ch);
	return 0;
}

static void wrapper_entry_point(void *arg)
{
	struct wrapper_args *args = arg;
	void (*entry_point)(void*) = args->entry_point;
	arg = args->arg;
	wire_pool_entry_t *entry = args->entry;
	wire_pool_t *pool = args->pool;

	entry_point(arg);

	if (pool->block_count > 0) {
		pool->block_count--;
		wire_channel_send(&pool->block_ch, entry);
	} else {
		list_add_head(&entry->list, &pool->free_list);
	}
}

static wire_t *wire_pool_entry_init(wire_pool_t *pool, wire_pool_entry_t *entry, const char *name, void (*entry_point)(void*), void *arg)
{
	VALGRIND_MAKE_MEM_UNDEFINED(entry->stack, pool->stack_size);
	VALGRIND_MAKE_MEM_UNDEFINED(&entry->task, sizeof(entry->task));

	/* This is a cool and ugly hack at the same time,
	 * the arguments are placed in the coroutine stack to avoid the need to
	 * allocate a new temporary space, it is placed at the end of the stack and
	 * the coroutine will copy it to the start of the stack at the beginning.
	 */

	struct wrapper_args *args = entry->stack;
	args->entry_point = entry_point;
	args->arg = arg;
	args->entry = entry;
	args->pool = pool;

	wire_init(&entry->task, name, wrapper_entry_point, args, entry->stack, pool->stack_size);
	return &entry->task;
}

wire_t *wire_pool_alloc(wire_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg)
{
	wire_pool_entry_t *entry;

	if (!list_empty(&pool->free_list)) {
		entry = list_entry(list_head(&pool->free_list), wire_pool_entry_t, list);
		list_del(&entry->list);
	} else if (pool->num_inited < pool->size) {
		// Not everything was initialized yet, initialize another one
		unsigned index = pool->num_inited++;
		entry = &(pool->entries[index]);
		if (!entry->stack)
			entry->stack = wire_stack_alloc(pool->stack_size);
	} else {
		// No place in pool
		return NULL;
	}

	return wire_pool_entry_init(pool, entry, name, entry_point, arg);
}

wire_t *wire_pool_alloc_block(wire_pool_t *pool, const char *name, void (*entry_point)(void*), void *arg)
{
	wire_t *wire;
	wire = wire_pool_alloc(pool, name, entry_point, arg);
	if (wire)
		return wire;

	wire_pool_entry_t *entry;
	pool->block_count++;
	wire_channel_recv_block(&pool->block_ch, (void**)&entry);
	return wire_pool_entry_init(pool, entry, name, entry_point, arg);
}
