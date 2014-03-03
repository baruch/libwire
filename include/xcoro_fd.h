#ifndef XCORO_FD_LIB_H
#define XCORO_FD_LIB_H

/** @file
 * XCoro file descriptor poll loop.
 *
 * This part of the library gives support for polling, sleeping and waiting
 * until a file descriptor is active and requires attention. It assumes that
 * the file descriptor is in a non-blocking state. It is expected to work for
 * pipes and sockets only and not for files as reading and writing to a real
 * file fd will result in a blocking action which should be avoided.
 */

/// @addtogroup FD Poller
/// @{

/** Initialize the XCoro file descriptor poller. This is currently global per app and should become local to an XCoro thread.
 */
void xcoro_fd_init(void);

/** One shot waiting for an FD to become readable. This is the less performant
 * cousing of the fd mode functions below. It will add and remove the fd from
 * the polling mechanism for each call.
 */
int xcoro_fd_wait_read(int fd);

/** Sleep in the poller for msecs time. The current coroutine is suspended and resumed back when the time passes. */
int xcoro_fd_wait_msec(int msecs);

typedef enum xcoro_fd_mode {
	FD_MODE_NONE,
	FD_MODE_READ,
	FD_MODE_WRITE,
} xcoro_fd_mode_e;

typedef struct xcoro_fd_state {
	int fd;
	xcoro_fd_mode_e state;
} xcoro_fd_state_t;

void xcoro_fd_mode_init(xcoro_fd_state_t *state, int fd);
int xcoro_fd_mode_read(xcoro_fd_state_t *fd_state);
int xcoro_fd_mode_write(xcoro_fd_state_t *fd_state);
int xcoro_fd_mode_none(xcoro_fd_state_t *fd_state);
void xcoro_fd_wait(xcoro_fd_state_t *fd_state);

/// @}

#endif
