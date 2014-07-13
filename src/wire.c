#include "wire.h"
#include "list.h"
#include "valgrind_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#ifdef WIRE_SWITCH_DEBUG
#include <unistd.h>
#endif

static wire_thread_t *g_wire_thread;

static wire_t *_wire_get_next(void)
{
	if (!list_empty(&g_wire_thread->ready_list)) {
		wire_t *wire = list_entry(list_head(&g_wire_thread->ready_list), wire_t, list);
		return wire;
	}

	// If we have nowhere to switch to, switch to the original starting stack
	// This will most likely result with us exiting from the application.
	return &g_wire_thread->sched_wire;
}

wire_t *wire_get_current(void)
{
	return g_wire_thread->running_wire;
}

static void _wire_switch_to(wire_t *wire)
{
	wire_t *from = wire_get_current();
	g_wire_thread->running_wire = wire;

	if (wire != from) {
#ifdef WIRE_SWITCH_DEBUG
		write(2, "Switching to wire ", strlen("Switching to wire "));
		write(2, wire->name, strlen(wire->name));
		write(2, "\n", 1);
#endif
		coro_transfer(&from->ctx, &wire->ctx);
	}
}

static void wire_schedule(void)
{
	_wire_switch_to(_wire_get_next());
}

static void _exec(wire_t *wire)
{
    wire->entry_point(wire->arg);

	// We exited from the wire, we shouldn't come back anymore
	list_del(&wire->list);

	// Invalidate memory in valgrind
	VALGRIND_MAKE_MEM_UNDEFINED(wire->stack, wire->stack_size);
	VALGRIND_MAKE_MEM_UNDEFINED(wire, sizeof(*wire));

	// Now switch to the next wire
	wire_schedule();

	// We should never get back here!
	abort();
}

void wire_thread_init(wire_thread_t *wire)
{
	g_wire_thread = wire;
	memset(g_wire_thread, 0, sizeof(*g_wire_thread));

	list_head_init(&g_wire_thread->ready_list);
	list_head_init(&g_wire_thread->suspend_list);
	sprintf(g_wire_thread->sched_wire.name, "sched %p", wire);

	// Initialize coro for the initial jump
	coro_create(&g_wire_thread->sched_wire.ctx, 0, 0, 0, 0);
	g_wire_thread->running_wire = &g_wire_thread->sched_wire;
}

void wire_thread_run(void)
{
	while (!list_empty(&g_wire_thread->ready_list)) {
		wire_schedule();
	}
}

wire_t *wire_init(wire_t *wire, const char *name, void (*entry_point)(void *), void *wire_data, void *stack, unsigned stack_size)
{
	memset(wire, 0, sizeof(*wire));

	strncpy(wire->name, name, sizeof(wire->name));
	wire->name[sizeof(wire->name)-1] = 0;

	wire->entry_point = entry_point;
	wire->arg = wire_data;
	wire->stack = stack;
	wire->stack_size = stack_size;

	coro_create(&wire->ctx, (coro_func)_exec, wire, stack, stack_size);
	list_add_tail(&wire->list, &g_wire_thread->ready_list);
	return wire;
}

void wire_yield(void)
{
	wire_t *this_wire = wire_get_current();

	// Move to the end of the ready list
	list_move_tail(&this_wire->list, &g_wire_thread->ready_list);

	// Schedule the next one (could be the same wire though!)
	wire_schedule();
}

void wire_suspend(void)
{
	wire_t *this_wire = wire_get_current();
	list_move_tail(&this_wire->list, &g_wire_thread->suspend_list);
	wire_schedule();
}

void wire_resume(wire_t *wire)
{
	list_move_tail(&wire->list, &g_wire_thread->ready_list);
}

int wire_is_only_one(void)
{
	return list_is_single(&g_wire_thread->ready_list);
}
