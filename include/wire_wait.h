#ifndef WIRE_LIB_WAIT_H
#define WIRE_LIB_WAIT_H
/** @file
 */

#include "list.h"
#include "wire.h"

/** @addtogroup wait Wait Facility
 * A Wait facility enables a wire to wait for different events, be they fd based or timers.
 */
/// @{

typedef struct wire_wait_list wire_wait_list_t;
typedef struct wire_wait wire_wait_t;

/** Initialize a wire waiting list.
 * @param[in] wl The wait list to initialize.
 */
void wire_wait_list_init(wire_wait_list_t *wl);

/** Initialize a wait before it can be used.
 *
 * @param[in] w The wait to initialize
 */
void wire_wait_init(wire_wait_t *w);

/** Chain another waiter to the list.
 *
 * @param[in] wl The waiter list to chain onto.
 * @param[in] w The waiter to chain into the list of waiters.
 */
void wire_wait_chain(wire_wait_list_t *wl, wire_wait_t *w);

/** Unchain a waiter from the list. This needs to be used when a waiter is no
 * longer needed on a list or needs to be moved to another list.
 * @param[in] w The waiter to unchain.
 */
void wire_wait_unchain(wire_wait_t *w);

/** Wait for at least one of the chained wait reasons to wake us up.
 *
 * @param[in] wl Wire list to wait on.
 * @return First wait reason that got woken, there may be more than one. The
 * caller should check for all of them in the linked list, can optimize to start at the returned item.
 */
wire_wait_t *wire_list_wait(wire_wait_list_t *wl);

/** Wake a waiter. This will wake a waiter if it isn't already triggered.
 *
 * @param[in] w Wait facility to wakeup.
 * @return Whether the resume did any work or was a no-op.
 */
int wire_wait_resume(wire_wait_t *w);

/** Reset a waiter to wait again. This is used to reset a triggered or stopped waiter to be marked again as waiting.
 * @param[in] w Wait facility to reset.
 */
void wire_wait_reset(wire_wait_t *w);

/** Cancel a waiter, a resume will no longer wake up this wire.
 * @param[in] w Waiter to cancel.
 */
void wire_wait_stop(wire_wait_t *w);

/** A simple waiter for just this one wait, often repeated so worth coding once for all.
 * @param[in] w The waiter to wait indefinitely for.
 */
void wire_wait_single(wire_wait_t *w);

/** Wait for two events at the same time, returns which waiter was the first to show up.
 * @param[in] wait1 First waiter
 * @param[in] wait2 Second waiter
 * @return 1 when wait1 finished first and 2 when wait2 finished first, -1 if none of them finished (improbable).
 */
int wire_wait_two(wire_wait_t *wait1, wire_wait_t *wait2);

/// @}

struct wire_wait_list {
	struct list_head head;
};

struct wire_wait {
	struct list_head list;
	wire_t *wire;
	unsigned waiting : 1;
	unsigned triggered : 1;
};

#endif
