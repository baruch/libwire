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

static void wire_schedule(void);
typedef void (*coro_func)(void *);

static __thread struct wire_thread g_wire_thread;

static void  __attribute__((used)) __attribute__((noreturn)) __wire_exit_c(wire_t* wire)
{
	// We exited from the wire, we shouldn't come back anymore
	list_del(&wire->list);
	//wire->cancelled = 0;

	// Invalidate memory in valgrind
	VALGRIND_MAKE_MEM_UNDEFINED(wire->stack, wire->stack_size);
	VALGRIND_MAKE_MEM_UNDEFINED(wire, sizeof(*wire));

	// Now switch to the next wire
	wire_schedule();

	// We should never get back here!
	abort();
}

#if !USE_LIBCORO

void coro_transfer(coro_context* from, coro_context* to);
asm (
   "\t.text\n"
   "coro_transfer:\n"

   #if __amd64
	   #define NUM_SAVED 6
	   "\tpushq %rbp\n"
	   "\tpushq %rbx\n"
	   "\tpushq %r12\n"
	   "\tpushq %r13\n"
	   "\tpushq %r14\n"
	   "\tpushq %r15\n"
	   "\tmovq %rsp, (%rdi)\n"
	   "\tmovq (%rsi), %rsp\n"
	   "\tpopq %r15\n"
	   "\tpopq %r14\n"
	   "\tpopq %r13\n"
	   "\tpopq %r12\n"
	   "\tpopq %rbx\n"
	   "\tpopq %rbp\n"
   #else
	 #error unsupported architecture
   #endif

	 "\tpopq %rcx\n"
	 "\tjmpq *%rcx\n"
);

/* This entry function will take the function to jump into and its argument
 * from the stack and call it in the x86-64 abi fashion. The return function will already be setup on the stack.
 *
 * It will then also read the wire argument from the stack and call the C function to do the final wire exit.
 */
void _wire_entry();
asm(
	".text\n"
	"_wire_entry:\n"
		"popq %rcx\n"
		"popq %rdi\n"
		"call *%rcx\n"
		"popq %rdi\n"
		"jmp __wire_exit_c\n"
);

void coro_create(coro_context* ctx, coro_func coro, void *arg, void *sptr, size_t ssize)
{
	if (!coro)
		return;

	ctx->sp = (void **)(ssize + (char *)sptr - sizeof(void*));
	*--ctx->sp = (wire_t*)ctx; // This means that ctx and wire must start at the same place
	*--ctx->sp = arg;
	*--ctx->sp = (void *)coro;
	*--ctx->sp = (void*)_wire_entry;

	ctx->sp -= NUM_SAVED;
	memset (ctx->sp, 0, sizeof (*ctx->sp) * NUM_SAVED);
}

#else /* USE_LIBCORO */

static void _exec(void* arg)
{
	wire_t* wire = arg;
	wire->entry_point(wire->arg);
	__wire_exit_c(wire);
}

#endif

static wire_t *_wire_get_next(void)
{
	if (__builtin_expect(!list_empty(&g_wire_thread.ready_list), 1)) {
		wire_t *wire = list_entry(list_head(&g_wire_thread.ready_list), wire_t, list);
		return wire;
	}

	// If we have nowhere to switch to, switch to the original starting stack
	// This will most likely result with us exiting from the application.
	return &g_wire_thread.sched_wire;
}

wire_t *wire_get_current(void)
{
	return g_wire_thread.running_wire;
}

static void wire_schedule(void)
{
	wire_t *wire = _wire_get_next();
	wire_t *from = wire_get_current();

	if (__builtin_expect(wire != from, 1)) {
#ifdef WIRE_SWITCH_DEBUG
		write(2, "Switching to wire ", strlen("Switching to wire "));
		write(2, wire->name, strlen(wire->name));
		write(2, "\n", 1);
#endif
		g_wire_thread.running_wire = wire;
		coro_transfer(&from->ctx, &wire->ctx);
		if (__builtin_expect(from->cancelled, 0)) {
#if WIRE_DEFER_SUPPORTED
			while (from->cleanup_head) {
				from->cleanup_head->cleanup(from->cleanup_head);
			}
#endif
			__wire_exit_c(from);
		}
	}
}

void wire_cancel(wire_t* wire)
{
	wire->cancelled = 1;
}

void wire_thread_init(void)
{
	memset(&g_wire_thread, 0, sizeof(g_wire_thread));

	list_head_init(&g_wire_thread.ready_list);
	list_head_init(&g_wire_thread.suspend_list);
	sprintf(g_wire_thread.sched_wire.name, "sched %p", &g_wire_thread);

	// Initialize coro for the initial jump
	coro_create(&g_wire_thread.sched_wire.ctx, 0, 0, 0, 0);
	g_wire_thread.running_wire = &g_wire_thread.sched_wire;
}

void wire_thread_run(void)
{
	wire_schedule();
}

wire_t *wire_init(wire_t *wire, const char *name, void (*entry_point)(void *), void *wire_data, void *stack, unsigned stack_size)
{
	memset(wire, 0, sizeof(*wire));

	strncpy(wire->name, name, sizeof(wire->name)-1);

#ifdef USE_VALGRIND
	wire->stack = stack;
	wire->stack_size = stack_size;
#endif

#if USE_LIBCORO
	wire->entry_point = entry_point;
	wire->arg = wire_data;
	coro_create(&wire->ctx, _exec, wire, stack, stack_size);
#else
	coro_create(&wire->ctx, (coro_func)entry_point, wire_data, stack, stack_size);
#endif
	list_add_tail(&wire->list, &g_wire_thread.ready_list);
	return wire;
}

void wire_yield(void)
{
	wire_t *this_wire = wire_get_current();

	// Move to the end of the ready list
	list_move_tail(&this_wire->list, &g_wire_thread.ready_list);

	// Schedule the next one (could be the same wire though!)
	wire_schedule();
}

void wire_suspend(void)
{
	wire_t *this_wire = wire_get_current();
	list_move_tail(&this_wire->list, &g_wire_thread.suspend_list);
	wire_schedule();
}

void wire_resume(wire_t *wire)
{
	list_move_tail(&wire->list, &g_wire_thread.ready_list);
}

int wire_is_only_one(void)
{
	return list_is_single(&g_wire_thread.ready_list);
}
