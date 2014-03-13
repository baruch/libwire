/** @file
 */
#ifndef WIRE_LIB_H
#define WIRE_LIB_H

/** @addtogroup Core
 * libwire main part. This includes the inner core of the libwire library,
 * everything else is only supporting this to make it easier to use and can be
 * replaced if needed.
 */
/// @{

/** The coroutine root.
 *
 * When libwire will support multiple threads there will be one wire_thread_t for each OS thread.
 */
typedef struct wire_thread wire_thread_t;

/** The user-space thread. This represents an actual user-space thread with all of its associated gear.
 * It can be allocted by the user or brought it by the wire_pool.
 */
typedef struct wire wire_t;

/** Initialize a wire thread.
 *
 * Give it an uninitialized but allocated space to work in and it will take care of it for this OS thread.
 */
void wire_thread_init(wire_thread_t *wire);

/** Run the user-space threads. This function only returns when there are no
 * wires to run, you should have initialized one or more wires before
 * calling it.
 */
void wire_thread_run(void);

/** Yield the CPU to the other wires. It will put the current running wire
 * to the end of the ready list and will let all the other wires get a slice of
 * the CPU too.
 */
void wire_yield(void);

/** Get the current running wire. Useful for when you need to give a wire to another function. */
wire_t *wire_get_current(void);

/** Checks if the current running wire is the only one.
 *
 * This is mostly useful for a polling manager to see if it needs to sleep in the OS or to only check for new actions.
 */
int wire_is_only_one(void);

/** Resumes a suspended wire. Puts the wire at the end of the ready list.
 *
 * A side effect of its action is to put a wire at the end of the ready list even if it is already in the running list.
 */
void wire_resume(wire_t *wire);

/** Suspend the currently running wire.
 */
void wire_suspend(void);

/** Initialize a wire and puts it on the ready list.
 *
 * You should give it an allocated wire to initialize, the name of the wire,
 * the entry function and an argument as well as the stack and the stack size.
 *
 * The wire will be put at the end of the ready list and will be run later on,
 * the current wire is not suspended or removed from the running list so if you
 * are in a long busy loop you should release the CPU with wire_yield().
 */
wire_t *wire_init(wire_t *wire, const char *name, void (*entry_point)(void *), void *arg, void *stack, unsigned stack_size);

/// @}

#include "wire_private.h"

#endif
