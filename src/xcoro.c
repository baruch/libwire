#include "wire.h"
#include "list.h"
#include "valgrind_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

static wire_t *g_wire;

static wire_task_t *_wire_get_next_task(void)
{
	if (!list_empty(&g_wire->ready_list)) {
		wire_task_t *task = list_entry(list_head(&g_wire->ready_list), wire_task_t, list);
		return task;
	}

	// If we have nowhere to switch to, switch to the original starting stack
	// This will most likely result with us exiting from the application.
	return &g_wire->sched_task;
}

wire_task_t *wire_get_current_task(void)
{
	return g_wire->running_task;
}

static void _wire_switch_to(wire_task_t *task)
{
	wire_task_t *from = wire_get_current_task();
	g_wire->running_task = task;

	if (task != from)
		coro_transfer(&from->ctx, &task->ctx);
}

static void wire_schedule(void)
{
	_wire_switch_to(_wire_get_next_task());
}

static void _exec(wire_task_t *task)
{
    task->entry_point(task->arg);

	// We exited from the task, we shouldn't come back anymore
	list_del(&task->list);

	// Invalidate memory in valgrind
	VALGRIND_MAKE_MEM_UNDEFINED(task->stack, task->stack_size);
	VALGRIND_MAKE_MEM_UNDEFINED(task, sizeof(*task));

	// Now switch to the next task
	wire_schedule();

	// We should never get back here!
	abort();
}

void wire_init(wire_t *wire)
{
	g_wire = wire;
	memset(g_wire, 0, sizeof(*g_wire));

	list_head_init(&g_wire->ready_list);
	list_head_init(&g_wire->suspend_list);
	sprintf(g_wire->sched_task.name, "sched %p", wire);

	// Initialize coro for the initial jump
	coro_create(&g_wire->sched_task.ctx, 0, 0, 0, 0);
	g_wire->running_task = &g_wire->sched_task;
}

void wire_run(void)
{
	while (!list_empty(&g_wire->ready_list)) {
		wire_schedule();
	}
}

wire_task_t *wire_task_init(wire_task_t *task, const char *name, void (*entry_point)(void *), void *task_data, void *stack, unsigned stack_size)
{
	memset(task, 0, sizeof(*task));

	strncpy(task->name, name, sizeof(task->name));
	task->name[sizeof(task->name)-1] = 0;

	task->entry_point = entry_point;
	task->arg = task_data;
	task->stack = stack;
	task->stack_size = stack_size;

	coro_create(&task->ctx, (coro_func)_exec, task, stack, stack_size);
	list_add_tail(&task->list, &g_wire->ready_list);
	return task;
}

void wire_yield(void)
{
	wire_task_t *this_task = wire_get_current_task();

	// Move to the end of the ready list
	list_move_tail(&this_task->list, &g_wire->ready_list);

	// Schedule the next one (could be the same task though!)
	wire_schedule();
}

void wire_suspend(void)
{
	wire_task_t *this_task = wire_get_current_task();
	list_move_tail(&this_task->list, &g_wire->suspend_list);
	wire_schedule();
}

void wire_resume(wire_task_t *task)
{
	list_move_tail(&task->list, &g_wire->ready_list);
}

int wire_is_only_task(void)
{
	return list_is_single(&g_wire->ready_list);
}
