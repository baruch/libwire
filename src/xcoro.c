#include "xcoro.h"
#include "list.h"
#include "valgrind_internal.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

static xcoro_t *g_xcoro;

int _switch(struct cpu_ctx *new_ctx, struct cpu_ctx *cur_ctx);
#ifdef __i386__
__asm__ (
" .text \n"
" .p2align 2,,3 \n"
".globl _switch \n"
"_switch: \n"
"__switch: \n"
"movl 8(%esp), %edx # fs->%edx \n"
"movl %esp, 0(%edx) # save esp \n"
"movl %ebp, 4(%edx) # save ebp \n"
"movl (%esp), %eax # save eip \n"
"movl %eax, 8(%edx) \n"
"movl %ebx, 12(%edx) # save ebx,esi,edi \n"
"movl %esi, 16(%edx) \n"
"movl %edi, 20(%edx) \n"
"movl 4(%esp), %edx # ts->%edx \n"
"movl 20(%edx), %edi # restore ebx,esi,edi \n"
"movl 16(%edx), %esi \n"
"movl 12(%edx), %ebx \n"
"movl 0(%edx), %esp # restore esp \n"
"movl 4(%edx), %ebp # restore ebp \n"
"movl 8(%edx), %eax # restore eip \n"
"movl %eax, (%esp) \n"
"ret \n"
);
#elif defined(__x86_64__)

__asm__ (
" .text \n"
" .p2align 4,,15 \n"
".globl _switch \n"
".globl __switch \n"
"_switch: \n"
"__switch: \n"
" movq %rsp, 0(%rsi) # save stack_pointer \n"
" movq %rbp, 8(%rsi) # save frame_pointer \n"
" movq (%rsp), %rax # save insn_pointer \n"
" movq %rax, 16(%rsi) \n"
" movq %rbx, 24(%rsi) # save rbx,r12-r15 \n"
" movq %r12, 32(%rsi) \n"
" movq %r13, 40(%rsi) \n"
" movq %r14, 48(%rsi) \n"
" movq %r15, 56(%rsi) \n"
" movq 56(%rdi), %r15 \n"
" movq 48(%rdi), %r14 \n"
" movq 40(%rdi), %r13 # restore rbx,r12-r15 \n"
" movq 32(%rdi), %r12 \n"
" movq 24(%rdi), %rbx \n"
" movq 8(%rdi), %rbp # restore frame_pointer \n"
" movq 0(%rdi), %rsp # restore stack_pointer \n"
" movq 16(%rdi), %rax # restore insn_pointer \n"
" movq %rax, (%rsp) \n"
" ret \n"
);
#endif

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
		_switch(&task->ctx, &from->ctx);
}

static void xcoro_schedule(void)
{
	_xcoro_switch_to(_xcoro_get_next_task());
}

static void _exec(xcoro_task_t *task)
{

#if defined(__llvm__) && defined(__x86_64__)
  __asm__ ("movq 16(%%rbp), %[task]" : [task] "=r" (task));
#endif
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

static void _xcoro_task_init(xcoro_task_t *task)
{
    void **stack = NULL;
    stack = (void **)(task->stack + (task->stack_size));

    stack[-3] = NULL;
    stack[-2] = (void *)task;
    task->ctx.esp = (void *)stack - (4 * sizeof(void *));
    task->ctx.ebp = (void *)stack - (3 * sizeof(void *));
    task->ctx.eip = (void *)_exec;
}

void xcoro_init(xcoro_t *xcoro)
{
	g_xcoro = xcoro;
	memset(g_xcoro, 0, sizeof(*g_xcoro));

	list_head_init(&g_xcoro->ready_list);
	list_head_init(&g_xcoro->suspend_list);
	sprintf(g_xcoro->sched_task.name, "sched %p", xcoro);
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

	_xcoro_task_init(task);
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
