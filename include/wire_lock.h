#ifndef WIRE_LIB_LOCK_H
#define WIRE_LIB_LOCK_H
/** @file
 */

#include "list.h"
#include "wire.h"

/** @addtogroup lock Lock Facility
 * A Lock facility enables a wire to lock a shared resource when it needs to be
 * held for more than a single wire cycle. An example is when waiting for a
 * socket to become writable, one wants to have only one wire writing into a
 * stream socket (i.e. TCP) so the data stream will be consistent even if the
 * writes performed by the wire take multiple waits to complete.
 *
 * The lock is fair in that all the pending lockers will get it in FIFO order.
 * In addition if a lock is requested twice and then released once, if another
 * locker comes in before the next lock user gets to it the new user will still
 * wait for the lock so this gives a complete FIFO order to the lock users.
 *
 * A wire_wait_t is used internally to wait on the lock so that when the lock
 * is released the wire that released it will not need to context switch to the
 * lock receiver and be able to continue the work it still needs to do. This
 * reduces the number of context switches that we need to do.
 */
/// @{

typedef struct wire_lock wire_lock_t;

/** Initialize a lock before it can be used.
 *
 * @param[in] l The lock to initialize
 */
void wire_lock_init(wire_lock_t *l);

/** Take a lock, will yield the wire if the lock cannot be taken and will wake
 * up once the lock is given to this wire.
 *
 * @param[in] l The lock to take.
 */
void wire_lock_take(wire_lock_t *l);

/** Release the lock, if there is a queue behind us, give the lock to the oldest waiter.
 * @param[in] l The lock to release.
 */
void wire_lock_release(wire_lock_t *l);

/** Wait for the lock to be emptied of all users. This can be used when closing
 * down the shared resource to wait for all users to finish.
 * @param[in] l The lock to wait on.
 */
void wire_lock_wait_clear(wire_lock_t *l);

/// @}

struct wire_lock {
	int num_users;
	struct list_head users;
};

#endif
