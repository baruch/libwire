#ifndef WIRE_TIMEOUT_LIB_H
#define WIRE_TIMEOUT_LIB_H

#include "wire_fd.h"
#include "wire_wait.h"

/** @file
 * libwire timeout handling.
 */

/** @defgroup Timeout Manage timing out actions
 *
 * wire_timeout.h takes care of timeout handling by creating a timeout state
 * which can be reset and used in wire_net.h for example.
 */
/// @{

/** Timeout state object.
 */
typedef struct wire_timeout {
	wire_fd_state_t fd_state;
	wire_wait_t wait;
} wire_timeout_t;

/** Initialize a timeout object, it will do nothing until wire_timeout_reset is called on it.
 * @param[in] tout The timeout object to initalize.
 * @return 0 on success, -1 on error.
 */
int wire_timeout_init(wire_timeout_t *tout);

/** Reset the timeout, restarting the count afresh.
 * @param[in] tout The timeout object to reset. It must be initialized already.
 * @param[in] timeout_msec New timeout in milliseconds.
 * @return 0 on success, -1 on error.
 */
int wire_timeout_reset(wire_timeout_t *tout, int timeout_msec);

/** Close a timeout, stopping its ticking and forgetting about it. The timeout object must be reset to be reused.
 * @param[in] tout The timeout object to stop.
 */
void wire_timeout_stop(wire_timeout_t *tout);

/** Get the wire_wait_t object that is underlying the implementation. This is needed to wait on the timeout by the user.
 * @param[in] tout The timeout object.
 * @return wire_wait_t object for the user to wait on.
 */
static inline wire_wait_t *wire_timeout_wait_get(wire_timeout_t *out) { return &out->fd_state.wait; }

/** Prime the timeout to be polled for.
 * @param[in] tout The timeout object.
 */
void wire_timeout_wait_start(wire_timeout_t *out);

/** Pause the polling of the timeout.
 * @param[in] tout The timeout object.
 */
void wire_timeout_wait_stop(wire_timeout_t *out);

/** Wait for a wait event and a timeout, returns which of them returned first.
 * @param[in] wait The wait object to wait on.
 * @param[in] tout The timeout object to use for timeout.
 * @return 1 if the wait object returned first, 2 if the timeout object returned first.
 */
int wire_timeout_wait(wire_wait_t *wait, wire_timeout_t *tout);

/// @}

#endif
