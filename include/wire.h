/** @file
 */
#ifndef WIRE_LIB_H
#define WIRE_LIB_H

/** @addtogroup Core
 * XCoro main part. This includes the inner core of the XCoro library,
 * everything else is only supporting this to make it easier to use and can be
 * replaced if needed.
 */
/// @{

/** The coroutine root.
 *
 * When wire will support multiple threads there will be one wire_t for each coroutine thread.
 */
typedef struct wire wire_t;

/** The coroutine task. This represents an actual coroutine with all of its associated gear.
 * It can be allocted by the user or brought it by the wire_task_pool.
 */
typedef struct wire_task wire_task_t;

/** Initialize an XCoro coroutine thread.
 *
 * Give it an uninitialized but allocated space to work in and it will take care of it for this thread.
 */
void wire_init(wire_t *wire);

/** Run the coroutine threads. This function only returns when there are no
 * coroutines to run, you should have initialized one or more coroutines before
 * calling it.
 */
void wire_run(void);

/** Yield the CPU to the other coroutines. It will put the current running task
 * to the end of the ready list and will let all the other tasks get a slice of
 * the CPU too.
 */
void wire_yield(void);

/** Get the current running task. Useful for when you need to give a task to some other XCoro function. */
wire_task_t *wire_get_current_task(void);

/** Checks if the current running task is the only one.
 *
 * This is mostly useful for a polling manager to see if it needs to sleep in the OS or to only check for new actions.
 */
int wire_is_only_task(void);

/** Resumes a suspended task. Puts the task at the end of the ready list.
 *
 * A side effect of its action is to put a task at the end of the ready list even if it is already in the running list.
 */
void wire_resume(wire_task_t *task);

/** Suspend the currently running task.
 */
void wire_suspend(void);

/** Initialize a task and puts it on the ready list.
 *
 * You should give it an allocated task to initialize, the name of the task,
 * the entry function and an argument as well as the stack and the stack size.
 *
 * The task will be put at the end of the ready list and will be run later on,
 * the current task is not suspended or removed from the running list so if you
 * are in a long busy loop you should release the CPU with wire_yield().
 */
wire_task_t *wire_task_init(wire_task_t *task, const char *name, void (*entry_point)(void *), void *task_data, void *stack, unsigned stack_size);

/// @}

#include "wire_private.h"

#endif
