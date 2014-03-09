#include "xcoro.h"
#include "list.h"
#include "valgrind_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

static xcoro_t *g_xcoro;

static xcoro_task_t *_xcoro_get_next_task(void)
{
	if (!list_empty(&g_xcoro->ready_list)) {
		xcoro_task_t *task = list_entry(list_head(&g_xcoro->ready_list), xcoro_task_t, list);
		return task;
	}

	// If we have nowhere to switch to, switch to the original starting stack
	// This will most likely result with us exiting from the application.
	return &g_xcoro->sched_task;
}

xcoro_task_t *xcoro_get_current_task(void)
{
	return g_xcoro->running_task;
}

static void _xcoro_switch_to(xcoro_task_t *task)
{
	xcoro_task_t *from = xcoro_get_current_task();
	g_xcoro->running_task = task;

	if (task != from)
		coro_transfer(&from->ctx, &task->ctx);
}

static void xcoro_schedule(void)
{
	_xcoro_switch_to(_xcoro_get_next_task());
}

static void _exec(xcoro_task_t *task)
{
    task->entry_point(task->arg);

	// We exited from the task, we shouldn't come back anymore
	list_del(&task->list);

	// Invalidate memory in valgrind
	VALGRIND_MAKE_MEM_UNDEFINED(task->stack, task->stack_size);
	VALGRIND_MAKE_MEM_UNDEFINED(task, sizeof(*task));

	// Now switch to the next task
	xcoro_schedule();

	// We should never get back here!
	abort();
}

void xcoro_init(xcoro_t *xcoro)
{
	g_xcoro = xcoro;
	memset(g_xcoro, 0, sizeof(*g_xcoro));

	list_head_init(&g_xcoro->ready_list);
	list_head_init(&g_xcoro->suspend_list);
	sprintf(g_xcoro->sched_task.name, "sched %p", xcoro);

	// Initialize coro for the initial jump
	coro_create(&g_xcoro->sched_task.ctx, 0, 0, 0, 0);
	g_xcoro->running_task = &g_xcoro->sched_task;
}

void xcoro_run(void)
{
	while (!list_empty(&g_xcoro->ready_list)) {
		xcoro_schedule();
	}
}

xcoro_task_t *xcoro_task_init(xcoro_task_t *task, const char *name, void (*entry_point)(void *), void *task_data, void *stack, unsigned stack_size)
{
	memset(task, 0, sizeof(*task));

	strncpy(task->name, name, sizeof(task->name));
	task->name[sizeof(task->name)-1] = 0;

	task->entry_point = entry_point;
	task->arg = task_data;
	task->stack = stack;
	task->stack_size = stack_size;

	coro_create(&task->ctx, (coro_func)_exec, task, stack, stack_size);
	list_add_tail(&task->list, &g_xcoro->ready_list);
	return task;
}

void xcoro_yield(void)
{
	xcoro_task_t *this_task = xcoro_get_current_task();

	// Move to the end of the ready list
	list_move_tail(&this_task->list, &g_xcoro->ready_list);

	// Schedule the next one (could be the same task though!)
	xcoro_schedule();
}

void xcoro_suspend(void)
{
	xcoro_task_t *this_task = xcoro_get_current_task();
	list_move_tail(&this_task->list, &g_xcoro->suspend_list);
	xcoro_schedule();
}

void xcoro_resume(xcoro_task_t *task)
{
	list_move_tail(&task->list, &g_xcoro->ready_list);
}

int xcoro_is_only_task(void)
{
	return list_is_single(&g_xcoro->ready_list);
}
